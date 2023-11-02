#include "rcpch.h"

#include "Services/Server/ServerGui.h"

#include "imgui.h"

namespace RC {

	ServerGui::ServerGui()
		: Service()
	{
		m_guiService = std::make_shared<SkeletonGui>();
		this->m_dependencies.push_back(
			DependencyDescriber("GUI", m_guiService, false)
		);

		m_serverService = Server::Create(ServerInput());
		this->m_dependencies.push_back(
			DependencyDescriber("SERVER", m_serverService, false)
		);
	}

	void ServerGui::Init()
	{
	}

	void ServerGui::Run()
	{
		while(true){
			ImGui::Begin("Settings");

			static bool m_show = true;
			ImGui::Checkbox("Checkbox", &m_show);

			ImGui::End();
			m_guiService->End();
		}
	}
}
