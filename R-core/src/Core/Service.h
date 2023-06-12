#pragma once
#include "rcpch.h"

#include "Core/Data.h"

namespace RC {

	class Service {
	public:

		virtual ~Service() = default;

		virtual void Init() = 0;
		virtual void OnUpdate() = 0;

		virtual inline void 
			AddDependencyCallback(std::function<void(Dispatchable&)> dependency)
		{
			m_dependencyCallbacks.emplace_back(dependency);
		};

		virtual inline const std::string& GetServiceName() { return m_serviceName; }

	protected:

		/* 
		* Have to be initialized in the Service's implementation constructor!
		* This callbacks should be used depending on the thread safety, they will
		* access memory from different service's classes that may be accesed by different threads.
		*/
		std::vector<std::function<void(Dispatchable&)>> m_dependencyCallbacks;
		std::string m_serviceName;
	};
}