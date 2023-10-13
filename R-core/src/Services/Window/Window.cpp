#include "rcpch.h"

#include "Services/Window/Window.h"
#include "Services/Window/Platform/WindowsWindow.h"

namespace RC {

	Window::Window(const WindowInput& input) : Service()
	{
		this->m_width = input.Width;
		this->m_height = input.Height;

	}

	std::shared_ptr<Window> Window::Create(const WindowInput& input)
	{
		#ifdef RC_PLATFORM_WINDOWS
			return std::make_shared<WindowsWindow>(input);
		#elif RC_PLATFORM_LINUX
			return std::make_shared<LinuxServer>(input);
		#else
			RC_LOG_ERROR("System not detected! Server couldn't be created");
			return nullptr;
		#endif	
	}

}
