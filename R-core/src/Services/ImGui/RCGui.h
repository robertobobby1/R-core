#pragma once

#include "Services/Window/Window.h"
#include "Core/Service.h"

namespace RC {

	class RCGui : public Service {
	public: 

		/*
		* For more information on how this works check imgui implementation
		* Specifically, example_glfw_opengl3 due to it being multiplatform
		*/
		RCGui();
		~RCGui();

		virtual void Init() override;
		virtual void OnUpdate() override;

		static std::shared_ptr<RCGui> Create();

		void Begin();
		void End();

	private:

		std::shared_ptr<Window> m_windowService;
	};
}