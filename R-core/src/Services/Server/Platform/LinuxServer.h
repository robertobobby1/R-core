#pragma once
#include "rcpch.h"

#include "Services/Server/Server.h"

namespace RC {

	class LinuxServer : public Server {
	public:
		LinuxServer(const ServerInput& input);
		~LinuxServer() = default;

		virtual void Run() override;
		void Setup();
		void OnError(const std::string &msg);

		void WorkerThreadLoop();

		void SetTSQueue(int socket);
		int GetTSQueue();

	private:

		// only to be used by main thread
		int m_fileDescriptor = 0;
		// Used by workers and main thread (needs to be protected with mutex)
		std::shared_ptr<std::queue<int>> m_socketQueue;
	};
}