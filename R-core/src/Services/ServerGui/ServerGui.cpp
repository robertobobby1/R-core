#include "rcpch.h"

#include "Services/ServerGui/ServerGui.h"

#include "imgui.h"

namespace RC {

	ServerGui::ServerGui()
		: Service()
	{
		m_guiService = std::make_shared<RCGui>();
		this->m_dependencies.push_back(
			DependencyDescriber("ImGui", m_guiService, false)
		);
	}

	void ServerGui::Init()
	{
	}

	void ServerGui::OnUpdate()
	{
		bool show_demo_window = true;
		m_guiService->Begin();
		//ImGui::ShowDemoWindow(&show_demo_window);
		m_guiService->End();
	}
}
