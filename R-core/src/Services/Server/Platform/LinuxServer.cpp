#include "rcpch.h"

#include "Core/Core.h"
#include "Services/Server/Server.h"
#include "Services/Server/Platform/LinuxServer.h"
#include "LinuxServer.h"

//#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)

namespace RC {

	LinuxServer::LinuxServer(const ServerInput& input)
		: Server(input)
	{
	}

    void LinuxServer::Run() 
    {
		Server::InitThreads(RC_BIND_FN(LinuxServer::WorkerThreadLoop));

		auto clientFileDescriptor = std::make_unique<int>();
		RC_LOG_INFO("Accepting requests in port {0}", m_data.m_port);
		while (!m_isShutdown){
			auto sizeSocket = sizeof(sockaddr_in);

			int clientSocket = accept(m_fileDescriptor, (sockaddr*)clientFileDescriptor.get(), (socklen_t *)&sizeSocket);
			if (clientSocket == -1){
				RC_LOG_WARN("Couldn't accept connection, retrying!");
				sleep(10);
				continue;
			}
            RC_LOG_INFO("New connection!");

            // Set SOCKET and notify one thread to handle it
            SetTSQueue(clientSocket);
			RC_LOG_INFO("setted");
            m_queueCondition.notify_one();
            m_data.m_totalConnections++;
		}
	}

	void LinuxServer::WorkerThreadLoop()
	{
		// create a heap buffer that will receive constantly
		int maxBufferLength = 512;
		Buffer buffer(maxBufferLength);
		int result = 0;

		bool openConexion = true;
		
		int clientSocket;
		while (openConexion)
		{
			// Blocking until the thread gets a task (New conexion)
			int clientSocket = GetTSQueue();
			Server::IncrementActiveConexions();
			RC_LOG_INFO("read");
			int result = read(clientSocket, buffer.Begin(), maxBufferLength);
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
				OnError("Conexion finished with error!");
				openConexion = false;
				Server::ReduceActiveConexions();
			}
			Service::CallDepCallbacks(m_data);
		}
	}

    void LinuxServer::Setup()
    {
		// Create a socket
     	m_fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		RC_LOG_INFO(m_fileDescriptor);

		sockaddr_in serverAddress;
		// Set server address port and ip address in struct
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(m_data.m_port);
		serverAddress.sin_addr.s_addr = INADDR_ANY;

		// Bind the ip address and port to a socket
		int iResult = bind(m_fileDescriptor, (sockaddr*)&serverAddress, sizeof(serverAddress));
		if (iResult == -1){
			RC_LOG_ERROR("Bind failed, port is probably in use!");
			m_isShutdown = true;
			return;
		}
		// strart listening on selected  port and ip address
		iResult = listen(m_fileDescriptor, m_data.m_backlog);
		if (iResult == -1){
			RC_LOG_ERROR("Listen failed");
			m_isShutdown = true;
			return;
		}
	}

	void LinuxServer::SetTSQueue(int socket)
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_socketQueue->push(socket);
    }

    int LinuxServer::GetTSQueue()
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_queueCondition.wait(lock);

        if (m_socketQueue->empty())
            return -1;

        int res = m_socketQueue->front();
        m_socketQueue->pop();

        return res;
    }

	void LinuxServer::OnError(const std::string &msg)
    {
        RC_LOG_ERROR(msg);
        Shutdown();
    }
}

//#endif