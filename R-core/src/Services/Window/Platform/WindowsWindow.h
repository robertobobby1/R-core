#pragma once

#include "Core/Service.h"
#include "Core/Data.h"

#include "Services/Window/Window.h"

namespace RC{
	class WindowsWindow : public Window
	{
	public: 
		WindowsWindow(const WindowInput& input);
		~WindowsWindow() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;

		void OnDispatchable(Dispatchable& data);
		void ServerCallbackFn(ServerData& serverData);
	};
}

