#include "rcpch.h"

#include "Services/Server/ServerGui.h"
#include "Core/Application.h"

#include "imgui.h"

namespace RC {

	ServerGui::ServerGui()
		: Service()
	{
		this->m_dependencies.push_back(
			DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false)
		);
		this->m_dependencies.push_back(
			DependencyDescriber("SERVER", Server::Create(ServerInput()), false)
		);
	}

	void ServerGui::Init()
	{
		m_guiService = DependencyDescriber::get<SkeletonGui>(this->m_dependencies, "GUI");
		m_serverService = DependencyDescriber::get<Server>(this->m_dependencies, "SERVER");

		// Added in init where the gui service is already initialized
		Application::GetApp().SetGuiRenderer(m_guiService);
	}

    void ServerGui::OnGuiUpdate() 
    {
		ImGui::Begin("Settings");

		static bool m_show = true;
		ImGui::Checkbox("Checkbox", &m_show);

		ImGui::End();
	}
}
