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
        void OnError(bool closeSocket = true);

        void SetTSQueue(SOCKET socket);
        SOCKET GetTSQueue();

       private:
        void Setup();

        // Only used by main thread
        SOCKET m_listenSocket;
        // Used by workers and main thread (needs to be protected with mutex)
        std::shared_ptr<std::queue<SOCKET>> m_socketQueue;
    };
}  // namespace RC
#endif