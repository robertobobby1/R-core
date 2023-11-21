project "GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("%{wks.location}/R-core/vendor/libs")
	objdir ("%{wks.location}/R-core/vendor/libs/bin/%{prj.name}")

	files
	{
		"include/GLFW/glfw3.h",
		"include/GLFW/glfw3native.h",
		"src/**.c",
		"src/**.m"
	}

	filter "system:macosx"
		defines "_GLFW_COCOA"

	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"
		defines "_GLFW_X11"

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"