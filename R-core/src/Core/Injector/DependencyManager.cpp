#include "rcpch.h"
#include "Core/Injector/DependencyManager.h"
#include "Core/Application.h"
#include "DependencyManager.h"

namespace RC {

    void DependencyManager::Run() {
        // add the dependencies in the services when needed
        AddDependencyServices();
        // crash when a circular dependency is found
        CheckNonCircularDependency();
    }

    bool DependencyManager::CheckDuplicatedUniqueService(std::shared_ptr<Service> service) {
        if (!service->IsUniqueService()) return false;

        std::vector<int> indexes = SearchServicesByClassName(service->GetChildClassName());

        if (indexes.size() > 1)
            RC_ASSERT_MSG(false,
                          "The search should not find more than 1, there are already duplicates!");

        // if already exists reassign given service to the already existing service
        if (indexes.size() == 1) {
            service = m_services[indexes[0]];
            return true;
        }

        return false;
    }

    void DependencyManager::AddDependencyServices() {
        // iterate all services incrementally
        for (auto& service : m_services) {
            // iterate the dependency vector using indexes
            for (int describerIndex = 0; describerIndex < service.second->m_dependencies.size();
                 describerIndex++) {
                auto& describer = service.second->m_dependencies[describerIndex];
                std::vector<int> indexes =
                    SearchServicesByClassName(describer.dep->GetChildClassName());
                // Use already instantiated services when its found by class name and:
                //		Dependency is unique service and hasn't been found yet
                //		Dependency is non unique service and has been found and try to find flag is
                //set
                if (describer.dep->IsUniqueService() && indexes.size() == 1 ||
                    !describer.dep->IsUniqueService() && describer.tryToFind &&
                        indexes.size() > 0) {
                    // change the services dependency to the new service, allowing him to access his
                    // dependencies
                    service.second->m_dependencies[describerIndex].dep = m_services[indexes[0]];
                }
                // Add the dependency as a new service when:
                //		Dependency is unique service and the service hasn't been found
                //		Dependency is non unique service and it wasn't found by class name or try to
                //find flag is not set
                else if (describer.dep->IsUniqueService() && indexes.size() == 0 ||
                         !describer.dep->IsUniqueService()) {
                    // will be added to the end of the list, dependencies will be checked anyways
                    Application::GetApp().AddService(describer.dep);
                    // On Any other case throw descriptive errors for debug purposes
                } else if (describer.dep->IsUniqueService() && indexes.size() > 1) {
                    RC_ASSERT_MSG(false,
                                  "Some error occured! Only one unique service should appear, "
                                  "check function RemoveDuplicatedUniqueServices");
                } else {
                    RC_ASSERT_MSG(false,
                                  "This error should be unaccesible, check unhandled option!")
                }
            }
        }
    }

    std::vector<int> DependencyManager::GetExecutionOrderIds() {
        std::vector<int> orderedList;

        // iterate all services dependencies and then add service if needed
        for (auto& service : m_services) {
            for (auto& dependency : service.second->m_dependencies) {
                AddDependencyDependencies(orderedList, dependency);
            }
            // check if it already exists in the vector
            bool found = false;
            for (auto& index : orderedList) {
                if (m_services[index]->GetId() == service.second->GetId()) found = true;
            }

            if (!found) orderedList.push_back(service.second->GetId());
        }
        return orderedList;
    }

    void DependencyManager::AddDependencyDependencies(std::vector<int>& orderedList,
                                                      DependencyDescriber& describer) {
        // base case: dependency already in the list (this searches in the vector for the dep)
        for (auto& index : orderedList) {
            if (m_services[index]->GetId() == describer.dep->GetId()) return;
        }

        // Add the dependencies dependencies first, as there should be no circular
        for (auto& dep : describer.dep->m_dependencies) {
            AddDependencyDependencies(orderedList, dep);
        }
        // finally add the dependency to the list
        orderedList.push_back(describer.dep->GetId());
    }
    std::vector<int> DependencyManager::SearchServicesByClassName(const char* className) {
        std::vector<int> res;
        for (auto& service : m_services) {
            // check if it has the same className
            if (*(service.second) != className) continue;

            res.push_back(service.second->m_id);
        }
        return res;
    }

    void DependencyManager::CheckNonCircularDependency() {
        for (auto& service : m_services) {
            // should only be a couple of dependencies, iterate the services dependencies
            for (auto& describer : service.second->m_dependencies) {
                // search by class name on services
                for (auto& dependencyService : m_services) {
                    // check if it doesn't have the same className and ignore them
                    if (*(service.second) != describer.dep->GetChildClassName()) continue;

                    // iterate over the dependencies of the service which is a dependency
                    for (auto& dependencyServiceDependencies :
                         dependencyService.second->m_dependencies) {
                        // assert it is not circular
                        RC_ASSERT_MSG((*(dependencyServiceDependencies.dep) !=
                                       service.second->GetChildClassName()),
                                      "Circular dependency {0} -> {1} -> {0} -> ...",
                                      service.second->GetChildClassName(),
                                      describer.dep->GetChildClassName())
                    }
                }
            }
        }
    }

}  // namespace RC