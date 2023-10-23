#pragma once

#include "Services/Imgui/RCGui.h"

namespace RC {
	class ServerGui : public Service {
	public:
		ServerGui();
		~ServerGui() = default;

		void Init() override;
		void OnUpdate() override;

	private: 
		std::shared_ptr<RCGui> m_guiService;
	};
}