#pragma once

#include <memory>
#include <iostream>
#include <iterator>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <queue>

// make sure the platform is correctly set
#include "Core/Platform.h"
#ifdef RC_PLATFORM_WINDOWS
	#include <WinSock2.h> 
	#include <ws2tcpip.h>
	#pragma comment( lib, "winmm.lib" )
	#pragma comment( lib, "WS2_32.lib" ) 
	#include <Windows.h>
#endif
