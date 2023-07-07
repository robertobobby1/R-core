
project "Testing"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/R-core/src",
		"%{wks.location}/R-core/vendor/imgui",
		"%{wks.location}/R-core/vendor/spdlog/include"
	}

	links
	{
		"R-core",
		"ImGui"
	}

	filter "system:windows"
	cppdialect "C++17"
	systemversion "latest"

	defines
	{
		"RC_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

		defines
		{
		}

		links
		{
		}

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

		defines
		{
		}

		links
		{
		}