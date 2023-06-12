project "R-Core"
	kind "StaticLib"
	language "C++"

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
	}

	includedirs
	{
		"src",
		"vendor/imgui",
		"vendor/spdlog/include/"
	}

	links
	{
		"ImGui",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"RC_PLATFORM_WINDOWS"
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
