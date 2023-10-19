#pragma once
#include "rcpch.h"

#include "Core/Data.h"
#include "uuid.h"

namespace RC {

	enum UniqueServiceType {
		USTNone = 0,
		UniqueWindow
	};

	// forward DependencyDescriber, it will be defined at the end of the file
	// DONT MOVE IT FROM THERE OR IT WILL CRASH AT EXECUTION
	struct DependencyDescriber;

	class Service {
	public:

		Service() = default;
		virtual ~Service() = default;
		virtual void Init() = 0;
		virtual void OnUpdate() = 0;

		virtual inline void 
			AddDependencyCallback(std::function<void(Dispatchable&)> dependency)
		{
			m_dependencyCallbacks.emplace_back(dependency);
		};

		virtual inline std::string ToString() {
			std::string res = "["; res += this->m_id; res += "-"; res += this->GetChildClassName(); res += "]";
			return res; 
		}

		virtual inline const char* GetChildClassName() const { return typeid(*(this)).name(); }
		virtual inline bool IsUniqueService()          const { return false; }
		virtual inline int GetId()                     const { return m_id; }
		 
		// Class operators to be able to use service to compare between them
		virtual inline bool operator< (int id) const                { return this->GetId() < id; }
		virtual inline bool operator> (int id) const                { return this->GetId() > id; }
		virtual inline bool operator==(int id) const                { return this->GetId() == id; }
		virtual inline bool operator!=(int id) const                { return this->GetId() != id; }
		virtual inline bool operator==(const char* className) const { return this->GetChildClassName() == className; }
		virtual inline bool operator!=(const char* className) const { return this->GetChildClassName() != className; }

	protected: 
		// id 0 is an invalid id, this has to be set by Application
		int m_id = 0;

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

		// give Application acces to the private objects in Service
		friend class Application;
	};

	struct DependencyDescriber
	{
		std::shared_ptr<Service> dep;
		bool tryToFind;
		const char* depName;

		DependencyDescriber(const char* _depName, std::shared_ptr<Service> _dependency, bool _tryToFind)
			: dep(_dependency), tryToFind(_tryToFind), depName(_depName){ }

		/*
		* Careful usage, the function "casts" the object for you by returning the children object 
		* however, it uses the given dependency name by the user to find it 
		*/
		template<typename T>
		static std::shared_ptr<T> get(std::vector<DependencyDescriber> dependencies, const char* serviceName)
		{
			for (auto& describer : dependencies) {
				if (describer.depName == serviceName)
					return describer.dep;
			}
			return nullptr;
		}
	};
}