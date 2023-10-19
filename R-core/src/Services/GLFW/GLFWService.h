#pragma once

#include "rcpch.h"
#include "Core/Service.h"


namespace RC {
	class GLFWService : public Service {
	public: 
		GLFWService() : Service() {}
		~GLFWService() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;

		static std::shared_ptr<GLFWService> Create();
	};
}