#pragma once

#include "Services/ImGui/RCGui.h"

namespace RC {

	class MacRCGui : public RCGui {
	public:
		MacRCGui();
		~MacRCGui() = default;

		virtual void Init() override;
		virtual void OnUpdate() override;
	};

}