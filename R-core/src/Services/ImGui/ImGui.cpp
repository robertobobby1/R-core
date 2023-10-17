#include "rcpch.h"

#include "Services/ImGui/ImGui.h"
#include "Services/Window/Window.h"

namespace RC {

	ImGui::ImGui() : Service()
	{
		// default values for window input, customize in the future
		DependencyDescriber depD(Window::Create(WindowInput()), false);
		this->m_dependencies.push_back(depD);
	}

	void ImGui::Init()
	{

	}

	void ImGui::OnUpdate() 
	{

	}
}