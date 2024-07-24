#include "rcpch.h"

#include "Core/Application.h"
#include "Services/ApplicationGui/InjectorGui.h"
#include <map>
#include "imgui.h"

namespace RC {

    static std::map<int, const char*> s_serviceNames;
    static std::map<int, const char*> s_isUniqueService;
    static std::map<int, const char*> s_isGuiService;
    static std::map<int, std::vector<int>> s_serviceDependencies;

    static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH |
                                   ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
                                   ImGuiTableFlags_NoBordersInBody;

    InjectorGui::InjectorGui() : Service() {
        this->m_dependencies.push_back(
            DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false));
        this->m_dockWindowName = "Dependency info";
    }

    void InjectorGui::Init() {
        m_guiService = this->GetDep<SkeletonGui>("GUI");
        // Added in init where the gui service is already initialized
        Application::GetApp().SetGuiRenderer(m_guiService);
        m_guiService->SetActiveDockWindow(m_dockWindowName, true);

        auto services = Application::GetApp().m_services;
        for (auto& service : services) {
            s_serviceNames[service.first] = service.second->GetChildClassName();
            s_isUniqueService[service.first] = service.second->IsUniqueService() ? "Yes" : "No";
            s_isGuiService[service.first] = service.second->IsGuiService() ? "Yes" : "No";

            std::vector<int> deps;
            for (auto& dependency : service.second->m_dependencies) {
                deps.push_back(dependency.dep->m_id);
            }
            s_serviceDependencies[service.first] = deps;
        }
    }

    void InjectorGui::OnGuiUpdate() {
        if (!m_guiService->GetActiveDockWindow(m_dockWindowName)) return;

        ImGui::Begin("Dependency Info");
        static float smallColumnWidth = ImGui::CalcTextSize("A").x * 6.0f;
        if (ImGui::BeginTable("3ColumnTable", 3, flags)) {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Unique", ImGuiTableColumnFlags_WidthFixed, smallColumnWidth);
            ImGui::TableSetupColumn("Gui", ImGuiTableColumnFlags_WidthFixed, smallColumnWidth);
            ImGui::TableHeadersRow();

            for (auto& service : s_serviceDependencies) {
                DisplayNode(service.first);
            }

            ImGui::EndTable();
        }
        ImGui::End();
    }

    void InjectorGui::DisplayNode(int Id) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        bool open = ImGui::TreeNodeEx(s_serviceNames[Id], tree_node_flags);
        ImGui::TableNextColumn();

        ImGui::TextUnformatted(s_isUniqueService[Id]);
        ImGui::TableNextColumn();

        ImGui::TextUnformatted(s_isGuiService[Id]);

        if (open) {
            for (auto dependencyId : s_serviceDependencies[Id]) {
                DisplayNodeDependencyInfo(dependencyId);
            }
            ImGui::TreePop();
        }
    }

    void InjectorGui::DisplayNodeDependencyInfo(int Id) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::TreeNodeEx(s_serviceNames[Id], tree_node_flags | ImGuiTreeNodeFlags_Leaf |
                                                  ImGuiTreeNodeFlags_Bullet |
                                                  ImGuiTreeNodeFlags_NoTreePushOnOpen);
        ImGui::TableNextColumn();
        ImGui::TextUnformatted(s_isUniqueService[Id]);
        ImGui::TableNextColumn();

        ImGui::TextUnformatted(s_isGuiService[Id]);
    }

}  // namespace RC
