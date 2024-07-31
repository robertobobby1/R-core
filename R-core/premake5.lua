absPathWorkspace = path.getabsolute("%{wks.location}")

project "R-Core"
	kind "StaticLib"
	cppdialect "C++20"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/")
	objdir ("%{wks.location}/bin/bin-int/" .. outputdir .. "/%{prj.name}")

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
		"STATIC_CONCPP",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}
	includedirs
	{
		absPathWorkspace .. "/R-core/src",
		absPathWorkspace .. "/R-core/vendor/imgui",
		absPathWorkspace .. "/R-core/vendor/spdlog/include",
		absPathWorkspace .. "/R-core/vendor/GLFW/include",
		absPathWorkspace .. "/R-core/vendor/glad/include",
		absPathWorkspace .. "/R-core/vendor/cpp-httplib"
	}
	libdirs
	{
		absPathWorkspace .. "/R-core/vendor/libs"
	}
	links
	{
		"GLFW",
		"ImGui",
		"glad"
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
