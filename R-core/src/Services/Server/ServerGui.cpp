#include "rcpch.h"

#include "Services/Server/ServerGui.h"
#include "Core/Utils/Data.h"
#include "Core/Application.h"
#include "Core/Core.h"

#include "imgui.h"

namespace RC {

    // static variables to be rendered by imgui
    static unsigned int s_activeConnections = 0;
    static unsigned int s_totalConnections = 0;
    static unsigned int s_port = 0;
    static unsigned int s_backlog = 0;
    static unsigned int s_workerThreads = 0;

    ServerGui::ServerGui() : Service() {
        this->m_dependencies.push_back(
            DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false)
        );
        this->m_dependencies.push_back(
            DependencyDescriber("SERVER", Server::Create(ServerInput()), false)
        );
        m_serverData = ServerData();
        m_dockWindowName = "Server";
    }

    void ServerGui::Init() {
        m_guiService = this->GetDep<SkeletonGui>("GUI");
        m_serverService = this->GetDep<Server>("SERVER");
        // receive changes in the server information to display
        m_serverService->AddDependencyCallback(RC_BIND_FN(ServerGui::OnDispatchable));

        // Added in init where the gui service is already initialized
        Application::GetApp().SetGuiRenderer(m_guiService);
        m_guiService->SetActiveDockWindow(m_dockWindowName, true);
    }

    void ServerGui::OnDispatchable(Dispatchable& dispatchable) {
        Dispatcher disp(dispatchable);
        disp.Dispatch<ServerData>([this](ServerData& serverData) {
            this->m_serverData = serverData;

            s_activeConnections = m_serverData.m_activeConnections;
            s_totalConnections = m_serverData.m_totalConnections;
            s_port = m_serverData.m_port;
            s_workerThreads = m_serverData.m_maxPoolSize;
            s_backlog = m_serverData.m_backlog;
        });
    }

    void ServerGui::OnGuiUpdate() {
        if (!m_guiService->GetActiveDockWindow(m_dockWindowName)) return;

        ImGui::Begin("ServerData");

        ImGui::Text("Active Connections: (%d)", s_activeConnections);
        ImGui::Text("Total Connections: (%d)", s_totalConnections);
        ImGui::Text("Port: (%d)", s_port);
        ImGui::Text("Backlog: (%d)", s_workerThreads);
        ImGui::Text("Number of worker threads: (%d)", s_workerThreads);

        ImGui::End();
    }
}  // namespace RC
