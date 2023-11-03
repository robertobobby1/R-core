#pragma once

#include "rcpch.h"

#include "Core/Platform.h"
#include "Core/Core.h"
#include "Core/DependencyManager.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Service.h"
#include "Core/Data.h"
#include "Core/Dispatch.h"
#include "Core/Buffer.h"

// Platform dependent services
#include "Services/Server/Server.h"
#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)
	#include "Services/Server/Platform/LinuxServer.h"
#elif defined(RC_PLATFORM_WINDOWS)
	#include "Services/Server/Platform/WindowsServer.h"
#endif

#include "Services/ImGui/RCGui.h"
#include "Services/ImGui/SkeletonGui.h"
#include "Services/Server/ServerGui.h"
#include "Services/Window/Window.h"
