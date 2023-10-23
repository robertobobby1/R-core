#pragma once

#include "Core/Service.h"

#include "GLFW/glfw3.h"

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
		~Window() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;

		static std::shared_ptr<Window> Create(const WindowInput& input);

		const char* m_title;
		uint32_t    m_width;
		uint32_t    m_height;
		bool	    m_Vsync;
		GLFWwindow* m_window;

	};
}