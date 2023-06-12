#pragma once
#include "rcpch.h"
#include "Core/Log.h"

namespace RC {

	// Memory allocation usage
	extern unsigned long long heapAllocatedBytes;
	static inline unsigned long long GetAllocations() { return heapAllocatedBytes; }

}

#define RC_DEBUGBREAK()         __debugbreak()

#define RC_LOG_INFO(...)        ::RC::Log::GetLogger()->info(__VA_ARGS__)
#define RC_LOG_WARN(...)	    ::RC::Log::GetLogger()->warn(__VA_ARGS__)
#define RC_LOG_DEBUG(...)	    ::RC::Log::GetLogger()->debug(__VA_ARGS__)
#define RC_LOG_ERROR(...)	    ::RC::Log::GetLogger()->error(__VA_ARGS__)
#define RC_LOG_CRITICAL(...)    ::RC::Log::GetLogger()->critical(__VA_ARGS__)

#define RC_LOG_MEMORYUSAGE()    ::RC::Log::GetLogger()->info("The current memory usage is {0}", (RC::GetAllocations() / 8))

#define RC_ASSERT_MSG(assert, ...)  { if(!assert) { RC_LOG_ERROR(__VA_ARGS__); RC_DEBUGBREAK();} }
#define RC_ASSERT(assert)  {    if(!assert) { RC_DEBUGBREAK();} }

#define RC_BIND_FN(fn)          [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
