#pragma once
#include "rcpch.h"

#include "Services/Server/Server.h"

#ifdef RC_PLATFORM_WINDOWS
namespace RC {

	class WindowsServer : public Server {
	public:
		WindowsServer(const ServerInput& input);
		~WindowsServer() = default;

		virtual void Run() override; 

		void WorkerThreadLoop();
		void OnError(const std::string& msg, bool closeSocket = true);
		
		void SetTSQueue(SOCKET socket);
		SOCKET GetTSQueue();

	private:
		bool Setup();

		// Only used by main thread
		SOCKET ListenSocket;
		// Used by workers and main thread (needs to be protected with mutex)
		std::shared_ptr<std::queue<SOCKET>> m_socketQueue;
	};
}
#endif