#include "rcpch.h"

#include "Core/Core.h"
#include "Core/Application.h"

#include "Services/Server/Server.h"
#include "Services/Server/Platform/WindowsServer.h"

#ifdef RC_PLATFORM_WINDOWS

namespace RC {

    WindowsServer::WindowsServer(const ServerInput& input)
        : Server(input)
    {
        m_socketQueue = std::make_shared<std::queue<SOCKET>>();
    }

    void WindowsServer::Run()
    {
        // Initializes threads in their loop functions
        Server::InitThreads(RC_BIND_FN(WindowsServer::WorkerThreadLoop));

        Setup();

        while (!m_isShutdown)
        {
            SOCKET AcceptSocket = accept(m_listenSocket, NULL, NULL);
            if (AcceptSocket == INVALID_SOCKET)
            {
                OnError("Accept failed with error:");
                return;
            }
            RC_LOG_INFO("New connection!");

            // Set SOCKET and notify one thread to handle it
            SetTSQueue(AcceptSocket);
            m_queueCondition.notify_one();
            m_data.m_totalConnections++;
        }
    }

    void WindowsServer::WorkerThreadLoop()
    {
        // create a heap buffer that will receive constantly
        int maxBufferLength = 512;
        Buffer buffer(maxBufferLength);
        int result = 0;

        bool openConexion = true;
        SOCKET clientSocket;

        while (true)
        {
            // Blocking until the thread gets a task (New conexion)
            clientSocket = GetTSQueue();
            Server::IncrementActiveConexions();
            while (openConexion)
            {
                result = recv(clientSocket, buffer.Begin(), maxBufferLength, 0);
                if (result > 0)
                {
                    RC_LOG_INFO("New Packet with {0} bytes", result);
                    if (f_onNewData)
                        f_onNewData(buffer);
                }
                else if (result == 0)
                {
                    RC_LOG_INFO("The peer closed the conexion!");
                    openConexion = false;
                    Server::ReduceActiveConexions();
                }
                else
                {
                    // maybe we shouldn't completely shutdown the server
                    OnError("Conexion finished with error:");
                    openConexion = false;
                    Server::ReduceActiveConexions();
                }
                Service::CallDepCallbacks(m_data);
            }
        }
    }

    void WindowsServer::SetTSQueue(SOCKET socket)
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_socketQueue->push(socket);
    }

    SOCKET WindowsServer::GetTSQueue()
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_queueCondition.wait(lock);

        if (m_socketQueue->empty())
            return -1;

        SOCKET res = m_socketQueue->front();
        m_socketQueue->pop();

        return res;
    }

    void WindowsServer::OnError(const std::string &msg, bool closeSocket)
    {
        RC_LOG_ERROR(msg, WSAGetLastError());

        if (closeSocket)
            closesocket(m_listenSocket);

        WSACleanup();
        Shutdown();
    }

    void WindowsServer::Setup()
    {
        // Initialize Winsock.
        WSADATA wsaData;

        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR)
        {
            RC_LOG_ERROR("WSAStartup failed with error: {0}", iResult);
            Shutdown();
            return;
        }

        // Create a SOCKET for listening
        m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_listenSocket == INVALID_SOCKET)
        {
            OnError("socket failed with error: {0}", false);
            return;
        }

        // Define the sockaddr_in structure
        sockaddr_in service;
        service.sin_family = AF_INET;
        service.sin_port = htons(27015);
        inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);
        if (bind(m_listenSocket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR)
        {
            OnError("bind failed with error: {0}");
            return;
        }

        // Listen for incoming connection requests.
        if (listen(m_listenSocket, 1) == SOCKET_ERROR)
        {
            OnError("listen failed with error: {0}");
            return;
        }

        // Set blocking mode to wait for connections
        unsigned long blocking_mode = 0;
        if (ioctlsocket(m_listenSocket, FIONBIO, &blocking_mode) == -1)
        {
            OnError("Couldn�t set to blocking mode! :: {0}");
            return;
        }
    }
}

#endif