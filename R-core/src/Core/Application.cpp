#include "rcpch.h"
#include "Core/Application.h"
#include "Core/Service.h"
#include "Core/Utils/Config.h"
#include "Core/Utils/Helper.h"
#include "Core/Injector/DependencyManager.h"
#include "Application.h"

namespace RC {

    // static global variables
    Application* Application::s_App = nullptr;
    std::map<size_t, std::string> LogFormatter::m_threadIdToServiceName;
    std::map<std::string, std::string> Config::s_config;

    Application::Application() {
        RC_ASSERT_MSG(!s_App, "App already instantiated!");
        s_App = this;

        // Initialize logger
        Log::Init(RC_BIND_FN(Application::LogCallback));
        LogFormatter::AddThreadName(RC_THREAD_ID(), "MainAppThread");

        // load config file
        Config::Load();
        PrintConfig();
    }

    void Application::Run() {
        m_depManager = new DependencyManager(m_services);
        m_depManager->Run();
        m_serviceOrder = m_depManager->GetExecutionOrderIds();

        PrintServices();

        // Initialize all services, synchronously and in dependency order
        for (auto& index : m_serviceOrder) {
            auto& service = m_services[index];
            service->Init();
            RC_LOG_INFO("The service {0} was succesfully initialized", service->ToString());
        }

        if (m_guiRenderer != nullptr) {
            m_guiRenderer->m_windowService->m_glfw->PrintOpenGLInfo();
        }

        // Run all services in independent threads
        for (auto& index : m_serviceOrder) {
            m_servicesThreads[index] = std::thread(
                [this](std::shared_ptr<Service> service) {
                    LogFormatter::AddThreadName(RC_THREAD_ID(), service->ToString().c_str());
                    RC_LOG_INFO("Starting...");
                    service->Run();
                },
                m_services[index]);
        }

        // UI needs to run in main thread!
        if (m_guiRenderer != nullptr) {
            RC_LOG_INFO("GUI rendering!");
            RunGui();
        }
        RC_LOG_INFO("No more GUI will be rendering");

        // Wait till all of them are finished
        for (auto& thread : m_servicesThreads) {
            RC_LOG_INFO("Waiting for service {0}", m_services[thread.first]->ToString());
            thread.second.join();
        }
    }

    void Application::RunGui() {
        while (m_isUiRunning) {
            m_guiRenderer->Begin();
            // check if polling events has closed the window
            if (!m_isUiRunning) return;
            for (auto& index : m_serviceOrder) {
                if (!m_services[index]->IsGuiService()) continue;
                m_services[index]->OnGuiUpdate();
            }
            m_guiRenderer->End();
        }
    }

    void Application::SetGuiRenderer(std::shared_ptr<GuiRenderer> guiRenderer) {
        if (m_guiRenderer != nullptr) {
            if (m_guiRenderer->GetId() == guiRenderer->GetId()) {
                return;
            }
            RC_LOG_WARN("Gui renderer was already set, if valid will overwrite!");
        }
        if (guiRenderer->GetId() == 0) {
            RC_LOG_ERROR("The gui renderer shared ptr must be initialized! No ID found!");
            return;
        }
        m_guiRenderer = guiRenderer;
    }

    void Application::LogCallback(const spdlog::details::log_msg& msg) {
        for (auto& callback : m_servicesLogCallbacks) {
            callback(msg);
        }
    }
}  // namespace RC