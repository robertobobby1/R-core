project "R-Core"
	kind "StaticLib"
	cppdialect "C++17"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rcpch.h"
	pchsource "src/rcpch.cpp"

	files
	{
		"src/**.cpp",
		"src/**.h"
	}
	
	defines 
	{
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	includedirs
	{
		"src",
		"vendor/imgui",
		"vendor/spdlog/include",
		"vendor/GLFW/include",
		"vendor/glad/include",
		"vendor/cpp-httplib"
	}
	links
	{
		"ImGui",
		"glad",
		"GLFW",
		"ssl"
	}

	filter "system:macosx"
		links 
		{
			"OpenGL.framework",
			"Cocoa.framework",
			"IOKit.framework"
		}

	filter "system:linux"
		systemversion "latest"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RC_RELEASE"
		runtime "Release"
		optimize "on"
