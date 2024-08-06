
project "Testing"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
 
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/")
	objdir ("%{wks.location}/bin/bin-int/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}
	files
	{
		"src/**.h",
		"src/**.cpp",
	}
	includedirs
	{
		"%{wks.location}/R-core/src",
		"%{wks.location}/R-core/vendor/imgui",
		"%{wks.location}/R-core/vendor/spdlog/include",
		"%{wks.location}/R-core/vendor/glfw/include",
		"%{wks.location}/R-core/vendor/glad/include",
		"%{wks.location}/R-core/vendor/cpp-httplib"
	}
	libdirs
	{
		"%{wks.location}/R-core/vendor/libs"
	}
	links
	{
		"GLFW",
		"ImGui",
		"glad",
		"R-core"
	}

	filter "system:macosx"
		links 
		{
			"OpenGL.framework",
			"Cocoa.framework",
			"IOKit.framework"
		}

	filter "configurations:Debug"
		defines "RC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RC_RELEASE"
		runtime "Release"
		optimize "on"
