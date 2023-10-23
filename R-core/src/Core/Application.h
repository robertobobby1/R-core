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

		// returns singleton
		inline static Application& GetApp() { return *s_App; }

		// will return the service id, add it to dep map and to service list
		inline void AddService(std::shared_ptr<Service> service)
		{
			if (CheckDuplicatedUniqueService(service))
				return;

			service->m_id = (int)(m_services.size() + 1);
			m_services[service->m_id] = service;
		}

		inline void RemoveServiceById(int id) 
		{
			m_services.erase(id); 
			SetExecutionOrderIds();
		}

		inline void PrintServices()
		{
			for (auto& service : m_services) {

				RC_LOG_INFO("{0}", service.second->ToString());
				for (auto& describer : service.second->m_dependencies){
					RC_LOG_INFO("----{0}", describer.dep->ToString(), describer.dep->m_id);
				}
			}
			RC_LOG_INFO("");
		}

	private:
		/*
		* Search service by className and return id of the service, 
		* will return the first one it finds, if not found return 0
		*/ 
		std::vector<int> SearchServicesByClassName(const char* className);
		/*
		* Check if the new given service is a duplicated and not 
		* add it if it is already inside and reassign the given shared ptr
		* Return if it was a duplicated unique service
		*/
		bool CheckDuplicatedUniqueService(std::shared_ptr<Service> service);
		/*
		* Will check there is no circular dependency meaning: given service 1 and service 2 
		* service1 -> service2 -> service1 -> service2 ... 
		* It has a very big temporal cost, however it should only run once 
		*/
		void CheckNonCircularDependency();
		/*
		* Will check all the dependencies of all services and instantiate them if necessary 
		* depending on every possible scenario: 
		* -----------| TryToFind | Non-instantiated | Instantiated | Multi-instantiated
		* Unique	 | --------- |        UC1       |      UC2     |	   ERROR     
		* Non-unique |	  true	 |        UC3       |      UC4     |		UC7
		* Non-unique |	  false	 |        UC5       |      UC6     |		UC8
		*/
		void AddDependencyServices();
		/*
		* Will return the order of execution to avoid services to be runned 
		* before their dependencies in a list of shared ptr services
		* THIS METHOD SHOULD ONLY BE CALLED AFTER DEPENDENCIES HAVE BEEN CORRECTLY INSTANTIATED
		*/
		void SetExecutionOrderIds();
		/*
		* Recursive function used to determine the order of the execution 
		* taking into account the dependencies
		*/
		void AddDependencyDependencies(std::vector<int>& m_orderedList, DependencyDescriber& dep);

	private:

		static Application* s_App;
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
	};
}