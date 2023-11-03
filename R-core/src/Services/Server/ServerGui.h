#pragma once

#include "Services/Imgui/SkeletonGui.h"
#include "Services/Server/Server.h"

namespace RC {
	class ServerGui : public Service {
	public:
		ServerGui();
		~ServerGui() = default;

		virtual void Init() override {};
		virtual void Run() override {};
		virtual void OnGuiUpdate() override;
		virtual inline bool IsGuiService() const { return true; }

	private: 
		std::shared_ptr<SkeletonGui> m_guiService;
		std::shared_ptr<Server> m_serverService;
	};
}