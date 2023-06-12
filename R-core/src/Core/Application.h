#pragma once

#include "rcpch.h"

#include "Core/Service.h"
#include "Core/Dispatch.h"

namespace RC {

	class Application {
	public:
		Application();
		~Application();

		// Inifinite loop
		void Run();

		inline static Application& GetApp() { return *s_App; }
		inline void AddService(std::shared_ptr<Service> service) { m_services.emplace_back(service); }

		void AddDependency(const std::string& serviceName, std::function<void(Dispatchable&)> callback);

	private:
		static Application* s_App;

		std::vector<std::shared_ptr<Service>> m_services;
	};
}