#include "rcpch.h"
#include "Core/Application.h"
#include "Core/Core.h"

namespace RC {

	Application* Application::s_App = nullptr;	

	Application::Application() 
	{
		RC_ASSERT_MSG(!s_App, "App already instanced!");
		s_App = this;

		Log::Init();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		// Initialize all services and start looping
		for (auto& service : m_services) {
			service->Init();
		}

		// infinite loop of application
		while (true) {
			// loop through all services, this vector should be limited
			for (auto& service : m_services) {
				service->OnUpdate();
			}
		}
	}

	/*
	* Add dependency to the service, the service can use it however it wants
	* Needs to be aware if the Service is thread safe or not, for non-thread safe
	* Services the dependency managing should be done by MainThread in OnUpdate
	*/
	void Application::AddDependency(const std::string& serviceName, std::function<void(Dispatchable&)> callback)
	{
		for (auto& service : m_services)
		{
			if (service->GetServiceName() == serviceName)
				service->AddDependencyCallback(callback);
		}
	}
}