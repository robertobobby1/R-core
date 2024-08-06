outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "R-core"
	cppdialect "C++20"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

include "R-core/vendor/imgui"
include "R-core/vendor/glfw"
include "R-core/vendor/glad"