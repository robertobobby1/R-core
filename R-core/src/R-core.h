#pragma once

#include "rcpch.h"

#include "Core/Platform.h"
#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Service.h"
#include "Core/Data.h"
#include "Core/Buffer.h"

#include "Services/Server/Server.h"

#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)
	#include "Services/Server/Platform/LinuxServer.h"
#elif defined(RC_PLATFORM_WINDOWS)
	#include "Services/Server/Platform/WindowsServer.h"
#endif

#include "Services/GLFW/GLFWService.h"
#include "Services/ImGui/ImGui.h"

#include "Services/Window/Window.h"
