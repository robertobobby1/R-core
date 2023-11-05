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
		virtual bool IsUniqueService() const override { return true; }
		
		void Shutdown();
		virtual void Begin();
		virtual void End();

		void OnDispatchable(Dispatchable& dispatchable);

	protected:

		std::shared_ptr<Window> m_windowService;

		bool m_isWindowRunning = true;
		bool m_isInitialized = false;
	};
}