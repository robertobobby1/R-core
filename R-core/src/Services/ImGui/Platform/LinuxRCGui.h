#pragma once

#include "Services/ImGui/RCGui.h"

namespace RC {

	class LinuxRCGui : public RCGui {
	public:
		LinuxRCGui();
		~LinuxRCGui() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;
	};

}