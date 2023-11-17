#pragma once
#include "rcpch.h"

#include "Core/Utils/Data.h"
#include "Core/Utils/Helper.h"
#include "Core/Injector/DependencyDescriber.h"

namespace RC {

	enum UniqueServiceType {
		USTNone = 0,
		UniqueWindow
	};

	class Service {
	public:

		Service() {};
		virtual ~Service() = default;
		//Application::GetApp().RemoveServiceById(this->GetId());

		// Keep as non virtual! It's not mandatory to implement the methods
		virtual void Init() {};
		virtual void Run() {};
		virtual void OnGuiUpdate() {};

		virtual inline void 
			AddDependencyCallback(std::function<void(Dispatchable&)> dependency) {
			std::lock_guard<std::mutex> lock(m_dataMutex);
			m_dependencyCallbacks.emplace_back(dependency);
		};

		virtual inline void CallDepCallbacks(Dispatchable& dispatchable) {
			std::lock_guard<std::mutex> lock(m_dataMutex);
			if (dispatchable.m_handled)
				return;

			for (auto& callback : this->m_dependencyCallbacks) {
				callback(dispatchable);
			}
			dispatchable.m_handled = true;
		}

		virtual inline std::string ToString() {
			std::string res = ""; res += to_string(this->m_id); res += "-"; res += this->GetChildClassName();
return res; 
		}

		virtual inline const char* GetChildClassName() const { return typeid(*(this)).name(); }
		virtual inline bool IsUniqueService()          const { return false; }
		virtual inline bool IsGuiService()             const { return false; }
		virtual inline int GetId()                     const { return m_id; }
		 
		// Class operators to be able to use service to compare between them
		virtual inline bool operator< (int id) const                { return this->GetId() < id; }
		virtual inline bool operator> (int id) const                { return this->GetId() > id; }
		virtual inline bool operator==(int id) const                { return this->GetId() == id; }
		virtual inline bool operator!=(int id) const                { return this->GetId() != id; }
		virtual inline bool operator==(const char* className) const { return this->GetChildClassName() == className; }
		virtual inline bool operator!=(const char* className) const { return this->GetChildClassName() != className; }

        /*
		* Careful usage, the function "casts" the object for you by returning the children object 
		* however, it uses the given dependency name by the user to find it 
		*/
		template<typename T>
		std::shared_ptr<T> GetDep(const char* serviceName)
		{
			for (auto& describer : this->m_dependencies) {
				if (describer.depName == serviceName)
					return std::dynamic_pointer_cast<T>(describer.dep);
			}
			return nullptr;
		}

	public: 
		// id 0 is an invalid id, this has to be set by Application
		int m_id = 0;

		// Mutex to access thread safely data
		std::mutex m_dataMutex;

		/* 
		* Services that need to access other services data must subscribe the dependency callback 
		* inside the init function (After application has adjusted all the dependencies)
		*/
		std::vector<std::function<void(Dispatchable&)>> m_dependencyCallbacks;

		/*
		* User should create a dependency describer for each service they depend on, 
		* they are forced to instantiate the needed service,
		* application will check the deps and change this object 
		*/
		std::vector<DependencyDescriber> m_dependencies;
	};
}