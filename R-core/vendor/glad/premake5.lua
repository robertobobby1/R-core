project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
	targetdir ("%{wks.location}/R-core/vendor/libs")
	objdir ("%{wks.location}/R-core/vendor/libs/bin/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"