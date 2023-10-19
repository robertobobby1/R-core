#include "rcpch.h"

#include "Services/ImGui/ImGui.h"
#include "Services/Window/Window.h"

namespace RC {

	ImGui::ImGui() : Service()
	{
		// default values for window input, customize in the future
		this->m_dependencies.push_back(DependencyDescriber(
			"Window", Window::Create(WindowInput()), true)
		);
	}

	void ImGui::Init()
	{
	}

	void ImGui::OnUpdate() 
	{
	}
}