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
		std::shared_ptr<Service> dependency;
		bool tryToFind;

		DependencyDescriber(std::shared_ptr<Service> _dependency, bool _tryToFind)
			: dependency(_dependency), tryToFind(_tryToFind) { }
	};

	class ServiceMapComparator {
	public: 
		using is_transparent = std::true_type;

		bool operator()(
			const int object1,
			const std::shared_ptr<RC::Service> object2
		) const {
			return object1 < object2->GetId();
		}

		bool operator()(
			const std::shared_ptr<RC::Service> object1,
			const int object2
		) const {
			return object1->GetId() < object2;
		}

		bool operator()(
			const std::shared_ptr<RC::Service> object1,
			const std::shared_ptr<RC::Service> object2
		) const {
			return object1->GetId() < object2->GetId();
		}
	};
}