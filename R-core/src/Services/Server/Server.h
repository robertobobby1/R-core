#pragma once

#include "Core/Service.h"
#include "Core/Utils/Buffer.h"

namespace RC {
	
	struct ServerInput {
		unsigned int Port;
		unsigned int MaxPoolSize;
		unsigned int Backlog;

		ServerInput(unsigned int port = 1000, unsigned int maxPoolSize = 3, unsigned int backlog = 100)
			: Port(port), MaxPoolSize(maxPoolSize), Backlog(backlog)
		{
		}

	};

	class Server : public Service {
	public:
		static std::shared_ptr<Server> Create(const ServerInput& input);

		inline void SetOnBufferDataCallback (std::function<void(Buffer&)> onNewDataFunc) 
			{ f_onNewData = onNewDataFunc; }

	protected:

		Server(const ServerInput& input);
		~Server() = default;

		virtual void Run() override = 0;

		virtual void InitThreads(std::function<void()> workerThreadFunc);
		virtual void Shutdown();

		void ReduceActiveConexions();
		void IncrementActiveConexions();

		// mutex and conditions
		std::mutex m_queueMutex;
		std::condition_variable m_queueCondition;

		// Shutdown flag
		bool m_isShutdown = false;

		// From here down variables need to be initialized externally 
		// By children objects or set functions 

		// Thread objects
		std::vector<std::thread> m_workerThreads;

		// Basic server data
		ServerData m_data;

		// Callback to new Data
		std::function<void(Buffer&)> f_onNewData;
	};
}