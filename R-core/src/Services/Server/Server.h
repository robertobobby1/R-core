#pragma once

#include "Core/Service.h"

namespace RC {
	
	struct ServerInput {
		uint32_t  Port;
		uint8_t  MaxPoolSize;

		ServerInput(uint32_t port = 1000, uint8_t maxPoolSize = 3)
			: Port(port), MaxPoolSize(maxPoolSize)
		{
		}

	};

	class Server : public Service {
	public:

		Server(const ServerInput& input);
		~Server() = default;

		virtual void Init() override = 0;
		virtual void OnUpdate() override;

		static std::shared_ptr<Server> Create(const ServerInput& input);

	protected:

		virtual void Server::InitThreads(std::function<void()> mainThreadFunc, std::function<void()> workerThreadFunc);

		// Thread objects
		std::shared_ptr<std::thread> m_mainThread;
		std::vector<std::shared_ptr<std::thread>> m_workerThreads;

		// mutex and conditions
		std::shared_ptr<std::mutex> mutex;
		std::shared_ptr<std::condition_variable> condition;

		// Basic server data
		std::shared_ptr<ServerData> m_data;
	};
}