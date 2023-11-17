#include "rcpch.h"

#include "Core/Application.h"
#include "Services/ApplicationGui/LogGui.h"

#include "imgui.h"

namespace RC {

    LogGui::LogGui() : Service(), m_autoScroll(true) {
        this->m_dependencies.push_back(
            DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false));

        this->m_dockWindowName = "Log";
    }

    void LogGui::Init() {
        m_guiService = this->GetDep<SkeletonGui>("GUI");
        // Added in init where the gui service is already initialized
        Application::GetApp().SetGuiRenderer(m_guiService);
        Application::GetApp().AddLogCallback(RC_BIND_FN(LogGui::AddLog));
        m_guiService->SetActiveDockWindow(m_dockWindowName, true);
    }

    void LogGui::OnGuiUpdate() {
        if (!m_guiService->GetActiveDockWindow(m_dockWindowName)) return;

        ImGui::Begin("Application logs");
        Draw();
        ImGui::End();
    }

    void LogGui::AddLog(const spdlog::details::log_msg& msg) {
        int old_size = m_buf.size();

        m_buf.append(msg.payload.data());

        for (int new_size = m_buf.size(); old_size < new_size; old_size++) {
            if (m_buf[old_size] == '\n') {
                m_lineOffsets.push_back(old_size + 1);
            }
        }
    }

    void LogGui::Draw() {
        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false,
                              ImGuiWindowFlags_HorizontalScrollbar)) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            const char* buf = m_buf.begin();
            const char* buf_end = m_buf.end();

            ImGuiListClipper clipper;
            clipper.Begin(m_lineOffsets.Size);

            while (clipper.Step()) {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                    const char* line_start = buf + m_lineOffsets[line_no];
                    const char* line_end = (line_no + 1 < m_lineOffsets.Size)
                                               ? (buf + m_lineOffsets[line_no + 1] - 1)
                                               : buf_end;

                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
            ImGui::PopStyleVar();

            if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::EndChild();
    }
}  // namespace RC
