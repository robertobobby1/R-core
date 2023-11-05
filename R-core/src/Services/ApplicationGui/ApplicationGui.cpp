#include "rcpch.h"

#include "Core/Application.h"
#include "Services/ApplicationGui/ApplicationGui.h"

#include "imgui.h"

namespace RC {
    ApplicationGui::ApplicationGui()
        : Service()
    {
        this->m_dependencies.push_back(
			DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false)
		);
    }

    void ApplicationGui::Init()
    {
        m_guiService = this->GetDep<SkeletonGui>("GUI");
        // Added in init where the gui service is already initialized
        Application::GetApp().SetGuiRenderer(m_guiService);
        m_guiService->SetActiveDockWindow("Application", true);
    }

    void ApplicationGui::OnGuiUpdate()
    {
        if (!m_guiService->GetActiveDockWindow("Application"))
            return;
        
        ImGui::Begin("Application Info");

        ImGui::End();

        static bool isOpen = true;
        ImGui::ShowDemoWindow(&isOpen);
    }
}
