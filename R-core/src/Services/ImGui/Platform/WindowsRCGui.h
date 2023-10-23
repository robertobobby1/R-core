#pragma once

#include "Services/ImGui/RCGui.h"

namespace RC {

	class WindowsRCGui : public RCGui {
	public:
		WindowsRCGui();
		~WindowsRCGui() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;
	};

}