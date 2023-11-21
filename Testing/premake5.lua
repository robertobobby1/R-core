
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
		"ImGui",
		"glad",
		"GLFW",
		"R-core",
		"ssl",
		"crypto",
		"pthread",
		"resolv"
	}

	filter "system:macosx"
		links 
		{
			"OpenGL.framework",
			"Cocoa.framework",
			"IOKit.framework"
		}
		libdirs
		{
			"%{wks.location}/R-core/vendor/mysql/MySQLMacOSARM/lib64"
		}
		linkoptions
		{
			"%{wks.location}/R-core/vendor/mysql/MySQLMacOSARM/lib64/libmysqlcppconn8-static.a"
		}