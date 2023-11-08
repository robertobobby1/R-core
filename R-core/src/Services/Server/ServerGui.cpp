#include "rcpch.h"

#include "Services/Server/ServerGui.h"
#include "Core/Utils/Data.h"
#include "Core/Application.h"

#include "imgui.h"

namespace RC {

	// static variables to be rendered by imgui
	static unsigned int activeConnections = 0;
	static unsigned int totalConnections  = 0;
	static unsigned int port  			  = 0;
	static unsigned int workerThreads     = 0;

	ServerGui::ServerGui()
		: Service()
	{
		this->m_dependencies.push_back(
			DependencyDescriber("GUI", std::make_shared<SkeletonGui>(), false)
		);
		this->m_dependencies.push_back(
			DependencyDescriber("SERVER", Server::Create(ServerInput()), false)
		);
		m_serverData = ServerData();
	}

	void ServerGui::Init()
	{
		m_guiService = this->GetDep<SkeletonGui>("GUI");
		m_serverService = this->GetDep<Server>("SERVER");
		// receive changes in the server information to display
		m_serverService->AddDependencyCallback(RC_BIND_FN(ServerGui::OnDispatchable));

		// Added in init where the gui service is already initialized
		Application::GetApp().SetGuiRenderer(m_guiService);
		m_guiService->SetActiveDockWindow("Server", true);
	}

	void ServerGui::OnDispatchable(Dispatchable& dispatchable)
	{
		Dispatcher disp(dispatchable);
		disp.Dispatch<ServerData>([this](ServerData& serverData){
			this->m_serverData = serverData;
			
			activeConnections = m_serverData.m_activeConnections;
			totalConnections  = m_serverData.m_totalConnections;
			port  = m_serverData.m_port;
			workerThreads = m_serverData.m_maxPoolSize;
		});
	}

    void ServerGui::OnGuiUpdate() 
    {
		if (!m_guiService->GetActiveDockWindow("Server"))
			return;

		ImGui::Begin("ServerData");

		ImGui::Text("Active Connections (%d)", activeConnections);
		ImGui::Text("Total Connections (%d)", totalConnections);
		ImGui::Text("Port (%d)", port);
		ImGui::Text("Number of worker threads (%d)", workerThreads);

		ImGui::End();
	}
}
