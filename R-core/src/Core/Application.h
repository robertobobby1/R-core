#pragma once
#include "rcpch.h"

#include "Core/Service.h"
#include "Core/Utils/Dispatch.h"
#include "Core/Core.h"
#include "Core/Injector/DependencyManager.h"

#include "Services/ImGui/GuiRenderer.h"

namespace RC {

    class Application {
       public:
        Application();
        ~Application() = default;
        void Run();

        // returns singleton
        inline static Application& GetApp() { return *s_App; }

        // will return the service id, add it to dep map and to service list
        inline void AddService(std::shared_ptr<Service> service) {
            if (m_depManager->CheckDuplicatedUniqueService(service)) return;

            service->m_id = (int)(m_services.size() + 1);
            m_services[service->m_id] = service;
        }

        inline void RemoveServiceById(int id) {
            m_services.erase(id);
            m_serviceOrder = m_depManager->GetExecutionOrderIds();
        }

        inline std::vector<std::string> AddLogCallback(LogCallbackFunction func) {
            m_servicesLogCallbacks.push_back(func);
            return m_startLogMessages;
        }

        inline void PrintServices() {
            RC_LOG_INFO("-------------- Services and dependency information ------------------");
            for (auto& service : m_services) {
                RC_LOG_INFO("{0}", service.second->ToString());
                for (auto& describer : service.second->m_dependencies) {
                    RC_LOG_INFO("----{0}", describer.dep->ToString());
                }
            }
            RC_LOG_INFO("-------------- End services and dependency information --------------");
        }

        void SetGuiRenderer(std::shared_ptr<GuiRenderer> guiRenderer);
        void LogCallback(const spdlog::details::log_msg msg);

       private:
        void RunGui();

       public:
        static Application* s_App;

        bool m_isUiRunning = true;
        DependencyManager* m_depManager;
        /*
         * keeps the service and a list of the dependencies that that service has
         * it is unordered and must be formatted during Run function because it is
         * used by the user by calling AddService multiple times
         */
        std::map<int, std::shared_ptr<Service>> m_services;
        /*
         * The order in which the services must be executed to keep the dependencies save
         */
        std::vector<int> m_serviceOrder;
        /*
         * Threads ordered by serviceId (each service is a thread)
         */
        std::map<int, std::thread> m_servicesThreads;

        std::shared_ptr<GuiRenderer> m_guiRenderer;
        std::vector<LogCallbackFunction> m_servicesLogCallbacks;
        // only used to keep first messages
        std::vector<std::string> m_startLogMessages;
        bool m_keepLogs = true;
    };
}  // namespace RC