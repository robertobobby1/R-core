outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "R-core"
	cppdialect "C++20"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	filter "system:macosx"
		architecture "universal"

	filter "system:windows"
		architecture "x86_64"
	
	filter "system:linux"
		architecture "x86_64"
		
include "Testing"
include "R-core"
