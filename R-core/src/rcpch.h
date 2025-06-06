#pragma once

#include <memory>
#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
#include <cmath>
#include <utility>
#include <queue>
#include <sstream>
#include <filesystem>

// make sure the platform is correctly set
#include "Core/Utils/Platform.h"
#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)
#    include <sys/socket.h>
#    include <netinet/in.h>
#    include <unistd.h>
#elif defined(RC_PLATFORM_WINDOWS)
#    include <WinSock2.h>
#    include <ws2tcpip.h>
#    pragma comment(lib, "winmm.lib")
#    pragma comment(lib, "WS2_32.lib")
#    include <Windows.h>
#endif
