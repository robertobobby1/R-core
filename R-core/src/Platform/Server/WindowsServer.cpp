#include "rcpch.h"

#include "Core/Core.h"
#include "Core/Application.h"

#include "Services/Server/Server.h"
#include "Platform/Server/WindowsServer.h"

namespace RC {

    WindowsServer::WindowsServer(const ServerInput& input) 
        : Server(input) 
    {
    }

    WindowsServer::~WindowsServer() { }

    void WindowsServer::Init()
    {
        // Initializes threads in their loop functions 
        Server::InitThreads(
            RC_BIND_FN(WindowsServer::MainThreadLoop),
            RC_BIND_FN(WindowsServer::WorkerThreadLoop)
        );

        RC_LOG_MEMORYUSAGE();
    }

	void WindowsServer::OnUpdate()
	{
        Server::OnUpdate();
	}

    void WindowsServer::MainThreadLoop()
    {
        //while(true){}
    }

    void WindowsServer::WorkerThreadLoop()
    {
        //while (true) {}
    }

    void WindowsServer::Setup()
    {
        // Initialize Winsock.
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
            RC_LOG_ERROR("WSAStartup failed with error: {0}", iResult);
            return;
        }

        // Create a SOCKET for listening
        SOCKET ListenSocket;
        ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ListenSocket == INVALID_SOCKET) {
            RC_LOG_ERROR("socket failed with error: {0}", WSAGetLastError());
            WSACleanup();
            return;
        }

        // Define the sockaddr_in structure
        sockaddr_in service;
        service.sin_family = AF_INET;
        service.sin_port = htons(27015);
        inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);

        iResult = bind(ListenSocket, (SOCKADDR*)&service, sizeof(service));
        if (iResult == SOCKET_ERROR) {
            RC_LOG_ERROR("bind failed with error: {0}", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        // Listen for incoming connection requests.
        if (listen(ListenSocket, 1) == SOCKET_ERROR) {
            RC_LOG_ERROR("listen failed with error: {0}", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        // Set blocking mode to wait for connections
        unsigned long blocking_mode = 0;
        iResult = ioctlsocket(ListenSocket, FIONBIO, &blocking_mode);
        RC_ASSERT_MSG((iResult != -1), "Couldn´t set to blocking mode!");
    }
}
