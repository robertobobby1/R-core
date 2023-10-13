#pragma once

#include "rcpch.h"

#include "Core/Service.h"
#include "Core/Dispatch.h"
#include "Core/Core.h"

namespace RC {

	class Application {
	public:
		Application();
		~Application();
		void Run();

		/*
		* Search service by className and return id of the service, 
		* will return the first one it finds, if not found return 0
		*/ 
		std::vector<int> SearchServicesByClassName(const char* className);
		/*
		* Remove all of the services that are unique and that may have been 
		* instantiated more than once due to it been dependency of multiple services
		*/
		void RemoveDuplicatedUniqueServices();
		/*
		* Will check there is no circular dependency meaning: given service 1 and service 2 
		* service1 -> service2 -> service1 -> service2 ... 
		* It has a very big temporal cost, however it should only run once 
		*/
		void CheckNonCircularDependency();
		/*
		* 
		*/
		void AddDependencyServices();

		// returns singleton
		inline static Application& GetApp() { return *s_App; }

		// will return the service id, add it to dep map and to service list
		inline void AddService(std::shared_ptr<Service> service)
		{
			service->m_id = (int)(m_services.size() + 1);
			m_services[service] = std::vector<int>();
		}

		inline void PrintServices()
		{
			for (auto& service : m_services) {
				RC_LOG_INFO("object of type {0}, with m_id {1} and has {2} service dependencies",
					service.first->GetChildClassName(), service.first->m_id, service.second.size()
				);
			}
		}

	private:

		static Application* s_App;

		/*
		* keeps the service and a list of the dependencies that that service has
		* it is unordered and must be formatted during Run function because it is
		* used by the user by calling AddService
		*/ 
		std::map<std::shared_ptr<Service>, std::vector<int>, ServiceMapComparator> m_services;

		/*
		* keeps the mapping between a deleted service during the Run function 
		* and the service it has been overwritten by, used for unique services and
		* non-unique that may be overwritten deleted services
		*/
		std::map<int, int> m_serviceOverwrite;

	};
}