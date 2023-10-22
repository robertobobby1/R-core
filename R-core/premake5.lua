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
	
	defines "GLFW_INCLUDE_NONE"

	includedirs
	{
		"src",

		"vendor/glad/include",
		"vendor/imgui",
		"vendor/spdlog/include/",
		"vendor/uuid",
		"vendor/glfw/include"
	}
	links
	{
		"ImGui",
		"glad",
		"GLFW"
	}

	filter "system:macosx"
		links 
		{
			"OpenGL.framework",
			"Cocoa.framework",
			"IOKit.framework"
		}
		defines 
		{
			"RC_PLATFORM_MACOS"
		}

	filter "system:linux"
		systemversion "latest"
		defines
		{
			"RC_PLATFORM_LINUX"
		}

	filter "system:windows"
		systemversion "latest"
		links
		{
			"Ws2_32.lib",
		}
		defines
		{
			"RC_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RC_RELEASE"
		runtime "Release"
		optimize "on"
