outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "R-core"
	cppdialect "C++20"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
		
include "Testing"
include "R-core"
