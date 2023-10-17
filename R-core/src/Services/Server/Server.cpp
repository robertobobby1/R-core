#include "rcpch.h"

#include "Core/Core.h"
#include "Core/Data.h"

#include "Services/Server/Server.h"
#include "Services/Server/Platform/WindowsServer.h"
#include "Services/Server/Platform/LinuxServer.h"

namespace RC {

	std::shared_ptr<Server> Server::Create(const ServerInput& input)
	{
		#ifdef RC_PLATFORM_WINDOWS
			return std::make_shared<WindowsServer>(input);
		#elif RC_PLATFORM_LINUX
			return std::make_shared<LinuxServer>(input);
		#else
			RC_LOG_ERROR("System not detected! Server couldn't be created");
			return nullptr;
		#endif	
	}

	Server::Server(const ServerInput& input) : Service()
	{
		this->m_data = std::make_shared<ServerData>();

		m_data->m_port = input.Port;
		m_data->m_maxPoolSize = input.MaxPoolSize;

	}

	void Server::InitThreads(std::function<void()> mainThreadFunc, std::function<void()> workerThreadFunc){

		m_mainThread = std::make_shared<std::thread>(mainThreadFunc);

		for (int i = 0; i < m_data->m_maxPoolSize; i++)
		{
			m_workerThreads.push_back(
				std::make_shared<std::thread>(workerThreadFunc)
			);
		}
	}

	void Server::Shutdown()
	{
		m_mainThread.get()->~thread();
		for (auto& thread : m_workerThreads)
		{
			thread.get()->~thread();
		}

		m_isShutdown = true;
		RC_LOG_WARN("Threads were terminated!");
	}

	void Server::OnUpdate()
	{
		if (m_data->m_handled)
			return;

		
		for (auto& depFunc : m_dependencyCallbacks)
		{
			depFunc(*(m_data.get()));
			m_data->m_handled = true;
		}
	}
}