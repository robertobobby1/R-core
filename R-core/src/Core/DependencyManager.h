#pragma once

#include "rcpch.h"
#include "Core/Service.h"

namespace RC {

    class Application;

    class DependencyManager {
    public: 
        
        static void Run();
        /*
		* Search service by className and return id of the service, 
		* will return the first one it finds, if not found return 0
		*/ 
		static std::vector<int> SearchServicesByClassName(const char* className);
		/*
		* Check if the new given service is a duplicated and not 
		* add it if it is already inside and reassign the given shared ptr
		* Return if it was a duplicated unique service
		*/
		static bool CheckDuplicatedUniqueService(std::shared_ptr<Service> service);
		/*
		* Will check there is no circular dependency meaning: given service 1 and service 2 
		* service1 -> service2 -> service1 -> service2 ... 
		* It has a very big temporal cost, however it should only run once 
		*/
		static void CheckNonCircularDependency();
		/*
		* Will check all the dependencies of all services and instantiate them if necessary 
		* depending on every possible scenario: 
		* -----------| TryToFind | Non-instantiated | Instantiated | Multi-instantiated
		* Unique	 | --------- |        UC1       |      UC2     |	   ERROR     
		* Non-unique |	  true	 |        UC3       |      UC4     |		UC7
		* Non-unique |	  false	 |        UC5       |      UC6     |		UC8
		*/
		static void AddDependencyServices();
		/*
		* Will return the order of execution to avoid services to be runned 
		* before their dependencies in a list of shared ptr services
		* THIS METHOD SHOULD ONLY BE CALLED AFTER DEPENDENCIES HAVE BEEN CORRECTLY INSTANTIATED
		*/
		static void SetExecutionOrderIds();
		/*
		* Recursive function used to determine the order of the execution 
		* taking into account the dependencies
		*/
		static void AddDependencyDependencies(std::vector<int>& m_orderedList, DependencyDescriber& dep);

        static Application& m_App;
    };
}