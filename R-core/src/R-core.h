#pragma once

#include "rcpch.h"

#include "Core/Utils/Platform.h"
#include "Core/Core.h"
#include "Core/Injector/DependencyManager.h"
#include "Core/Injector/DependencyDescriber.h"
#include "Core/Application.h"
#include "Core/Utils/Log.h"
#include "Core/Utils/Helper.h"
#include "Core/Service.h"
#include "Core/Utils/Data.h"
#include "Core/Utils/Dispatch.h"
#include "Core/Utils/Buffer.h"

// Platform dependent services
#include "Services/Server/Server.h"
#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)
#    include "Services/Server/Platform/LinuxServer.h"
#elif defined(RC_PLATFORM_WINDOWS)
#    include "Services/Server/Platform/WindowsServer.h"
#endif

#include "Services/ImGui/GuiRenderer.h"
#include "Services/ImGui/SkeletonGui.h"
#include "Services/Server/ServerGui.h"
#include "Services/HttpServer/HttpServer.h"
#include "Services/HttpServer/HttpLogging.h"
#include "Services/HttpServer/Controller.h"
#include "Services/HttpServer/Middleware.h"
#include "Services/HttpServer/Helper.h"
#include "Services/ApplicationGui/InjectorGui.h"
#include "Services/ApplicationGui/LogGui.h"
#include "Services/ApplicationGui/ApplicationGui.h"
#include "Services/FileSystem/FileSystem.h"
#include "Services/FileSystem/FileSystemGui.h"
#include "Services/Window/Window.h"
