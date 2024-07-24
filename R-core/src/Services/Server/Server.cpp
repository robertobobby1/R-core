#include "rcpch.h"

#include "Core/Core.h"
#include "Core/Utils/Data.h"

#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)
#    include "Services/Server/Platform/LinuxServer.h"
#elif defined(RC_PLATFORM_WINDOWS)
#    include "Services/Server/Platform/WindowsServer.h"
#endif

#include "Services/Server/Server.h"
#include "Server.h"

namespace RC {

    std::shared_ptr<Server> Server::Create(const ServerInput& input) {
#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)
        return std::make_shared<LinuxServer>(input);
#elif defined(RC_PLATFORM_WINDOWS)
        return std::make_shared<WindowsServer>(input);
#else
        RC_LOG_ERROR("System not detected! Server couldn't be created");
        return nullptr;
#endif
    }

    Server::Server(const ServerInput& input) : Service() {
        this->m_data = ServerData();

        m_data.m_port = input.Port;
        m_data.m_maxPoolSize = input.MaxPoolSize;
        m_data.m_backlog = input.Backlog;
    }

    void Server::InitThreads(std::function<void()> workerThreadFunc) {
        for (unsigned int i = 0; i < m_data.m_maxPoolSize; i++) {
            m_workerThreads.push_back(std::thread([this, workerThreadFunc, i]() {
                LogFormatter::AddThreadName(
                    RC_THREAD_ID(), fmt::format("{}-W{}", ToString(), to_string(i)).c_str());
                workerThreadFunc();
            }));
        }
    }

    void Server::Shutdown() {
        m_isShutdown = true;
        RC_LOG_WARN("Threads were terminated!");
    }

    void Server::ReduceActiveConexions() {
        {
            std::lock_guard<std::mutex> lock(m_dataMutex);
            m_data.m_activeConnections--;
            m_data.m_handled = false;
        }
        Service::CallDepCallbacks(m_data);
    }

    void Server::IncrementActiveConexions() {
        {
            std::lock_guard<std::mutex> lock(m_dataMutex);
            m_data.m_activeConnections++;
            m_data.m_handled = false;
        }
        Service::CallDepCallbacks(m_data);
    }

}  // namespace RC
