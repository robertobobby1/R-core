#include "rcpch.h"
#include "Core/Application.h"
#include "Core/Service.h"

namespace RC {

	Application* Application::s_App = nullptr;	

	Application::Application() 
	{
		RC_ASSERT_MSG(!s_App, "App already instantiated!");
		s_App = this;

		Log::Init();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		DependencyManager::Run();
		PrintServices(); 
		
		// Initialize all services, synchronously and in dependency order
		RC_LOG_INFO("-------------- Initializing application services -------------------");
		for (auto& index : m_serviceOrder) {
			auto& service = m_services[index];
			service->Init();
			RC_LOG_INFO("The service {0} with id {1} has a shared ptr count of {2} was succesfully initialized",
				service->GetChildClassName(), service->GetId(), service.use_count()
			);
		}
		RC_LOG_INFO("-------------- End initializing application services ---------------");

		// Run all services in independent threads
		for (auto& index : m_serviceOrder) {
/* 			m_servicesThreads[index] = std::thread([this](std::shared_ptr<Service> service) {
				service->Run();
			}, m_services[index]); */
		}

		// UI needs to run in main thread!
		if (m_guiRenderer != nullptr){
			RC_LOG_INFO("GUI rendering!");
			RunGui();
		}
		RC_LOG_INFO("No more GUI will be rendering");

		// Wait till all of them are finished
		for (auto& index : m_serviceOrder) {
			RC_LOG_INFO("Waiting for service {0}", m_services[index]->ToString());
			m_servicesThreads[index].join();
		}
	}

    void Application::RunGui()
    {
		while (m_isUiRunning){
			m_guiRenderer->Begin();
			for (auto& index : m_serviceOrder) {
				if (!m_services[index]->IsGuiService()) continue;
				m_services[index]->OnGuiUpdate();
			}
			m_guiRenderer->End();
		}
    }

    void Application::SetGuiRenderer(std::shared_ptr<RCGui> guiRenderer)
    {
		if (m_guiRenderer != nullptr){
			RC_LOG_WARN("Gui renderer was already set, if valid will overwrite!");
		}
		if (guiRenderer->GetId() == 0){
			RC_LOG_ERROR("The gui renderer shared ptr must be initialized! No ID found!");
			return;
		}
		m_guiRenderer = guiRenderer;
    }
}