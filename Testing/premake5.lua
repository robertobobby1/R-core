
project "Testing"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
 
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/")
	objdir ("%{wks.location}/bin/bin-int/" .. outputdir .. "/%{prj.name}")

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
		"%{wks.location}/R-core/vendor/GLFW/include",
		"%{wks.location}/R-core/vendor/glad/include",
		"%{wks.location}/R-core/vendor/cpp-httplib"
	}
	links
	{
		"R-core"
	}
	linkoptions
	{
		"%{wks.location}/R-core/vendor/libs/libGLFW.a",
		"%{wks.location}/R-core/vendor/libs/libImGui.a",
		"%{wks.location}/R-core/vendor/libs/libglad.a",
	}

	filter "system:macosx"
		links 
		{
			"OpenGL.framework",
			"Cocoa.framework",
			"IOKit.framework"
		}