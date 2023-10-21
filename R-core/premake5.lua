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
		"UUID_SYSTEM_GENERATOR",
		"GLFW_INCLUDE_NONE"
	}

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

		defines
		{
			"RC_PLATFORM_WINDOWS"
		}

		links
		{
			"Ws2_32.lib",
		}

	filter "configurations:Debug"
		defines "RC_DEBUG"
		runtime "Debug"
		symbols "on"

		defines
		{
		}

		links
		{
		}

	filter "configurations:Release"
		defines "RC_RELEASE"
		runtime "Release"
		optimize "on"

		defines
		{
		}

		links
		{
		}
