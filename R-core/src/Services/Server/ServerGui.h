#pragma once

#include "Services/Imgui/SkeletonGui.h"
#include "Services/Server/Server.h"

namespace RC {
	class ServerGui : public Service {
	public:
		ServerGui();
		~ServerGui() = default;

		void Init() override;
		void Run() override;

	private: 
		std::shared_ptr<SkeletonGui> m_guiService;
		std::shared_ptr<Server> m_serverService;
	};
}