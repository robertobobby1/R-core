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
		m_guiService = this->GetDep<SkeletonGui>("GUI");
		m_serverService = this->GetDep<Server>("SERVER");

		// Added in init where the gui service is already initialized
		Application::GetApp().SetGuiRenderer(m_guiService);
		m_guiService->SetActiveDockWindow("Server", true);
	}

    void ServerGui::OnGuiUpdate() 
    {
		if (!m_guiService->GetActiveDockWindow("Server"))
			return;

		ImGui::Begin("Settings");

		static bool m_show = true;
		ImGui::Checkbox("Checkbox", &m_show);

		ImGui::End();
	}
}
