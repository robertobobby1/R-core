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
		PrintServices();
		// Remove unique services duplications 
		RemoveDuplicatedUniqueServices();
		PrintServices(); 
		// add the dependencies in the services when needed 
		AddDependencyServices();
		PrintServices(); 
		// crash when a circular dependency is found
		CheckNonCircularDependency();

		// Initialize all services and start looping
		for (auto& service : m_services) {
			service.first->Init();
		}

		// infinite loop of application
		while (true) {
			// loop through all services, this vector should be limited
			for (auto& service : m_services) {
				service.first->OnUpdate();
			}
		}
	}

	void Application::RemoveDuplicatedUniqueServices()
	{
		for (auto& service : m_services) {
			if (!service.first->IsUniqueService())
				return;

			std::vector<int> indexes = SearchServicesByClassName(service.first->GetChildClassName());
			// remove all the duplicates
			for (auto& index : indexes) {
				if (index == service.first->m_id)
					continue;
				m_serviceOverwrite.insert({ index, service.first->m_id });
				m_services.erase(m_services.find(index));
			}
		}
	}

	void Application::AddDependencyServices()
	{
		// iterate all services incrementally
		for (auto& service : m_services) {
			RC_LOG_INFO("service with id {0}, checking its {1} dependencies", service.first->m_id , service.first->m_dependencies.size());
			// iterate the dependency vector using indexes 
			for (int dependencyIndex = 0; dependencyIndex < service.first->m_dependencies.size(); dependencyIndex++) {
				auto& dependency = service.first->m_dependencies[dependencyIndex];
				std::vector<int> indexes = SearchServicesByClassName(dependency.dependency->GetChildClassName());
				// Use already instantiated services when its found by class name and:
				//		Dependency is unique service and hasn't been found yet
				//		Dependency is non unique service and has been found and try to find flag is set
				if (
					dependency.dependency->IsUniqueService() && indexes.size() == 1 ||
					!dependency.dependency->IsUniqueService() && dependency.tryToFind && indexes.size() > 0
				) {
					// change the services dependency to the new service, allowing him to access his dependencies
					m_services
						.find(service.first->m_id)
						->first
						->m_dependencies[dependencyIndex]
						.dependency
						.reset(m_services.find(indexes[0])->first.get());
				}
				// Add the dependency as a new service when: 
				//		Dependency is unique service and the service hasn't been found
				//		Dependency is non unique service and it wasn't found by class name or try to find flag is not set
				else if (
					dependency.dependency->IsUniqueService() && indexes.size() == 0 ||
					!dependency.dependency->IsUniqueService()
				) {
					// will be added to the end of the list, dependencies will be checked anyways
					this->AddService(dependency.dependency);
				// On Any other case throw descriptive errors for debug purposes 
				} else if (dependency.dependency->IsUniqueService() && indexes.size() > 1) {
					RC_ASSERT_MSG(false,
						"Some error occured! Only one unique service should appear, check function RemoveDuplicatedUniqueServices"
					);
				} else {
					RC_ASSERT_MSG(false, "This error should be unaccesible, check unhandled option!")
				}
			}
		}
	}

	std::vector<int> Application::SearchServicesByClassName(const char* className)
	{
		std::vector<int> res;
		for (auto& service : m_services) {
			// check if it has the same className 
			if (*(service.first) != className)
				continue;

			res.push_back(service.first->m_id);
		}
		return res;
	}

	void Application::CheckNonCircularDependency()
	{
		for (auto& service : m_services) {
			// should only be a couple of dependencies, iterate the services dependencies
			for (auto& dependency : service.first->m_dependencies) {
				// search by class name on services
				for (auto& dependencyService : m_services) {
					// check if it doesn't have the same className and ignore them
					if (*(service.first) != dependency.dependency->GetChildClassName())
						continue;

					// iterate over the dependencies of the service which is a dependency 
					for (auto& dependencyServiceDependencies : dependencyService.first->m_dependencies) {
						// assert it is not circular
						RC_ASSERT_MSG(
							(*(dependencyServiceDependencies.dependency) != service.first->GetChildClassName()),
							"Circular dependency {0} -> {1} -> {0} -> ...",
							service.first->GetChildClassName(), dependency.dependency->GetChildClassName()
						)
					}
				}
			}
		}
	}
}