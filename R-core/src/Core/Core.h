#pragma once
#include "rcpch.h"

#include "Core/Utils/Log.h"

// Memory allocation usage
extern unsigned long long heapAllocatedBytes;
static inline unsigned long long GetAllocations() { return heapAllocatedBytes; }

#if defined(RC_PLATFORM_MACOS) || defined(RC_PLATFORM_LINUX)
	#include <signal.h>
	#define RC_DEBUGBREAK()         raise(SIGTRAP)
#elif defined(RC_PLATFORM_WINDOWS)
	#define RC_DEBUGBREAK()         __debugbreak()
#else
	#define RC_DEBUGBREAK()
#endif

#ifdef RC_DEBUG 
	#define RC_ASSERT_MSG(assert, ...)  { if(!assert) { RC_LOG_ERROR(__VA_ARGS__); RC_DEBUGBREAK();} }
	#define RC_ASSERT(assert)		    { if(!assert) { RC_DEBUGBREAK();} }

	#define RC_LOG_MEMORYUSAGE()    ::RC::Log::GetLogger()->info("The current memory usage is {0}", (GetAllocations() / 8))
	#define RC_LOG_DEBUG(...)	    ::RC::Log::GetLogger()->debug(__VA_ARGS__)
#else
	#define RC_ASSERT_MSG(assert, ...)  
	#define RC_ASSERT(assert)  

	#define RC_LOG_MEMORYUSAGE()    
	#define RC_LOG_DEBUG(...)	    
#endif 

#define RC_LOG_INFO(...)        ::RC::Log::GetLogger()->info(__VA_ARGS__)
#define RC_LOG_WARN(...)	    ::RC::Log::GetLogger()->warn(__VA_ARGS__)
#define RC_LOG_CRITICAL(...)    ::RC::Log::GetLogger()->critical(__VA_ARGS__)
#define RC_LOG_ERROR(...)	    ::RC::Log::GetLogger()->error(__VA_ARGS__)

#define RC_LOG_INFO(...)        ::RC::Log::GetLogger()->info(__VA_ARGS__)
#define RC_LOG_WARN(...)	    ::RC::Log::GetLogger()->warn(__VA_ARGS__)
#define RC_LOG_DEBUG(...)	    ::RC::Log::GetLogger()->debug(__VA_ARGS__)
#define RC_LOG_ERROR(...)	    ::RC::Log::GetLogger()->error(__VA_ARGS__)
#define RC_LOG_CRITICAL(...)    ::RC::Log::GetLogger()->critical(__VA_ARGS__)

#define RC_BIND_FN(fn)          [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }