#pragma once

#include "Core/Service.h"
#include "Core/Data.h"

#include "GLFW/glfw3.h"
#include "Services/GLFW/RCGlfw.h"

namespace RC {

	struct WindowInput {
		uint32_t  Height;
		uint32_t  Width;
		const char* Title;

		WindowInput(uint32_t width = 1600, uint32_t height = 900, const char* title = "Window")
			: Width(width), Height(height), Title(title)
		{
		}
	};

	class Window : public Service {
	public:
		Window(const WindowInput& input);
		~Window();

		virtual void Init() override;
		virtual void Run() override {};
		virtual void OnGuiUpdate() override {};
		virtual inline bool IsUniqueService() const { return true; }

		void SetVSync(bool _Vsync);

		static std::shared_ptr<Window> Create(const WindowInput& input);

		void OnDispatchable(Dispatchable& dispatchable);

		GLFWwindow* m_window;
		WindowData m_data;

	private: 
		std::shared_ptr<RCGlfw> m_glfw;
	};
}