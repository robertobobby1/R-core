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
#ifdef RC_PLATFORM_WINDOWS
#include "Services/ImGui/Platform/WindowsRCGui.h"
#elif defined(RC_PLATFORM_MACOS)
#include "Services/ImGui/Platform/MacRCGui.h"
#elif defined(RC_PLATFORM_LINUX)
#include "Services/ImGui/Platform/LinuxRCGui.h"
#endif

namespace RC
{

	std::shared_ptr<RCGui> RCGui::Create()
	{
#ifdef RC_PLATFORM_WINDOWS
		return std::make_shared<WindowsRCGui>();
#elif defined(RC_PLATFORM_MACOS)
		return std::make_shared<MacRCGui>();
#elif defined(RC_PLATFORM_LINUX)
		return std::make_shared<LinuxRCGui>();
#else
		RC_LOG_ERROR("The platform is not supported! Supported platforms are Linux, Windows, Mac");
		return nullptr;
#endif
	}

	RCGui::RCGui()
		: Service()
	{
		m_windowService = Window::Create(WindowInput());

		// default values for window input, customize in the future
		this->m_dependencies.push_back(DependencyDescriber(
			"Window", m_windowService, false));
	}

	RCGui::~RCGui()
	{
		// imgui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		// glfw
		glfwDestroyWindow(m_windowService->m_window);
		glfwTerminate();
		// app
		Application::GetApp().RemoveServiceById(m_windowService->GetId());
	}

	void RCGui::Init()
	{
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

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_windowService->m_window, true);
		// TODO Version 4.1 for now dynamic?
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void RCGui::OnUpdate()
	{
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
		io.DisplaySize = ImVec2((float)m_windowService->m_width, (float)m_windowService->m_height);

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

		glfwSwapBuffers(m_windowService->m_window);
	}
}