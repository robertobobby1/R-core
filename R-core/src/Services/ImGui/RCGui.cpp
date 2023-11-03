#include "rcpch.h"

#include "Core/Core.h"
#include "Core/Application.h"
#include "Services/Window/Window.h"

// IMGUI
#include "imgui.h"
#include "Services/ImGui/ImGuiBuild/imgui_impl_glfw.h"
#include "Services/ImGui/ImGuiBuild/imgui_impl_opengl3.h"

// GL
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Services/ImGui/RCGui.h"
#include "RCGui.h"

namespace RC
{

	RCGui::RCGui()
		: Service()
	{
		// default values for window input, customize in the future
		this->m_dependencies.push_back(DependencyDescriber(
			"Window", Window::Create(WindowInput()), false)
		);
		
	}

	void RCGui::Shutdown()
	{
		// imgui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		// glfw
		glfwTerminate();
		// will call window's destructor including deletion of glfw window's deletion
		m_windowService.reset();
	}

	RCGui::~RCGui()
	{
		if (m_isInitialized){
			Shutdown();
		}
	}

	void RCGui::Init()
	{
		m_windowService = DependencyDescriber::get<Window>(this->m_dependencies, "Window");
		m_windowService->AddDependencyCallback(RC_BIND_FN(RCGui::OnDispatchable));
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows
		// io.ConfigViewportsNoAutoMerge = true;
		// io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(m_windowService->m_window, true);
		// TODO Version 4.1 for now dynamic?
		ImGui_ImplOpenGL3_Init("#version 410");
		m_isInitialized = true;
	}

    void RCGui::Begin() 
    {
		glfwPollEvents();			
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RCGui::End()
	{
		ImGuiIO &io = ImGui::GetIO();
		// set size
		io.DisplaySize = ImVec2((float)m_windowService->m_data.m_width, (float)m_windowService->m_data.m_height);

		// Rendering
		ImGui::Render();

		int display_w, display_h;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		glfwGetFramebufferSize(m_windowService->m_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		// don't swap buffers if window is not running
		if (!m_isWindowRunning)
			return;

		glfwSwapBuffers(m_windowService->m_window);
	}

	void RCGui::OnDispatchable(Dispatchable& dispatchable)
	{
		Dispatcher disp(dispatchable);
		disp.Dispatch<OnWindowCloseEvent>([this](OnWindowCloseEvent& dispatchable) {
			this->m_isWindowRunning = false;
		});
	}
}