
project "Testing"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
 
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}
	defines 
	{
		"STATIC_CONCPP"
	}
	ignoredefaultlibraries
	{
		"libcmtd"
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
		"R-core",
		"ImGui",
		"glad",
		"GLFW"
	}

	filter "system:macosx"
		links
		{
			"OpenGL.framework",
			"Cocoa.framework",
			"IOKit.framework",
		}
		libdirs
		{
			"%{wks.location}/R-core/vendor/mysql/MySQLMacOSARM/lib64"
		}

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