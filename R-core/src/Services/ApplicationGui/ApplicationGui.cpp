#include "rcpch.h"

#include "Core/Application.h"
#include "Services/ApplicationGui/ApplicationGui.h"
#include "Services/ApplicationGui/LogGui.h"

#include "imgui.h"

namespace RC {
    ApplicationGui::ApplicationGui() : Service() {
        this->m_dependencies.push_back(
            DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false));
        // add log to the main application gui
        this->m_dependencies.push_back(
            DependencyDescriber("Log", std::make_shared<LogGui>(), false));
    }

    void ApplicationGui::Init() {
        m_guiService = this->GetDep<SkeletonGui>("GUI");
        // Added in init where the gui service is already initialized
        Application::GetApp().SetGuiRenderer(m_guiService);
    }

    void ApplicationGui::OnGuiUpdate() {
        static bool isOpen = true;
        ImGui::ShowDemoWindow(&isOpen);
    }
}  // namespace RC
