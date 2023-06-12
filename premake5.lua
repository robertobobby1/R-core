outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "R-core"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

include "R-core"
include "Testing"