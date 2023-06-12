#include "rcpch.h"

#include "Core/Application.h"
#include "Core/Core.h"
#include "Core/Service.h"

#include "Platform/Window/WindowsWindow.h"

namespace RC
{
	WindowsWindow::WindowsWindow(const WindowInput& input)
		: Window(input) 
	{
	}

	void WindowsWindow::Init()
	{
		Application::GetApp().AddDependency("Server", RC_BIND_FN(WindowsWindow::OnDispatchable));
	}

	void WindowsWindow::OnUpdate()
	{
	}

	void WindowsWindow::OnDispatchable(Dispatchable& dispachable)
	{
		Dispatcher dispatcher(dispachable);
		dispatcher.Dispatch<ServerData>(
			RC_BIND_FN(WindowsWindow::ServerCallbackFn)
		);
	}

	void WindowsWindow::ServerCallbackFn(ServerData& serverData)
	{
	}
}
