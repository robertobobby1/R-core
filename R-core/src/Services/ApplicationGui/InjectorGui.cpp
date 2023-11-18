#include "rcpch.h"

#include "Core/Application.h"
#include "Services/ApplicationGui/InjectorGui.h"
#include <map>
#include "imgui.h"

namespace RC {

    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

    static std::map<int, std::string> s_serviceNames;
    static std::map<int, bool> s_isUniqueService;
    static std::map<int, bool> s_isGuiService;
    static std::map<int, std::vector<int>> s_serviceDependencies;

    static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;
    static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH |
                                   ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
                                   ImGuiTableFlags_NoBordersInBody;

    InjectorGui::InjectorGui() : Service() {
        this->m_dependencies.push_back(
            DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false));
    }

    void InjectorGui::Init() {
        m_guiService = this->GetDep<SkeletonGui>("GUI");
        // Added in init where the gui service is already initialized
        Application::GetApp().SetGuiRenderer(m_guiService);

        /* auto services = Application::GetApp().m_services;
        for (auto& service : services) {
            s_isUniqueService[service.first] = service.second->IsUniqueService();
            s_isGuiService[service.first] = service.second->IsGuiService();

            std::vector<int> deps;
            for (auto& dependency : service.second->m_dependencies) {
                deps.push_back(dependency.dep->m_id);
            }
            s_serviceDependencies[service.first] = deps;
        } */
    }

    void InjectorGui::OnGuiUpdate() {
        if (ImGui::BeginTable("3ways", 3, flags)) {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Unique", ImGuiTableColumnFlags_WidthFixed,
                                    TEXT_BASE_WIDTH * 12.0f);
            ImGui::TableSetupColumn("Gui", ImGuiTableColumnFlags_WidthFixed,
                                    TEXT_BASE_WIDTH * 18.0f);
            ImGui::TableHeadersRow();

            for (auto& service : s_serviceDependencies) {
                DisplayNode(service.first);
            }

            ImGui::EndTable();
        }
    }

    void InjectorGui::DisplayNode(int Id) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        bool open = ImGui::TreeNodeEx(s_serviceNames[Id].c_str(), tree_node_flags);
        ImGui::TableNextColumn();

        ImGui::Checkbox("", &s_isUniqueService[Id]);
        ImGui::TableNextColumn();

        ImGui::Checkbox("", &s_isGuiService[Id]);
        ImGui::TableNextColumn();

        for (auto dependencyId : s_serviceDependencies[Id]) {
            DisplayNodeDependencyInfo(dependencyId);
        }
        ImGui::TreePop();
    }

    void InjectorGui::DisplayNodeDependencyInfo(int Id) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::TreeNodeEx(s_serviceNames[Id].c_str(), tree_node_flags | ImGuiTreeNodeFlags_Leaf |
                                                          ImGuiTreeNodeFlags_Bullet |
                                                          ImGuiTreeNodeFlags_NoTreePushOnOpen);
        ImGui::TableNextColumn();
        ImGui::Checkbox("", &s_isUniqueService[Id]);
        ImGui::TableNextColumn();

        ImGui::Checkbox("", &s_isGuiService[Id]);
        ImGui::TableNextColumn();
    }

}  // namespace RC
