#pragma once

#include "Services/Imgui/SkeletonGui.h"
#include "Services/Server/Server.h"

namespace RC {
	class ServerGui : public Service {
	public:
		ServerGui();
		~ServerGui() = default;

		virtual void Init() override;
		virtual void OnGuiUpdate() override;
		virtual inline bool IsGuiService() const override { return true; }

		void OnDispatchable(Dispatchable& dispatchable);

		std::shared_ptr<SkeletonGui> m_guiService;
		std::shared_ptr<Server> m_serverService;

		ServerData m_serverData;
	};
}