#pragma once
// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #pragma message "WIN32 || _WIN32 || __WIN32__ || __NT__"
    #define RC_PLATFORM_WINDOWS
    #ifdef _WIN64
        #pragma message "_WIN64"
    #else
        #pragma message "RC_NOT_WINDOWS"
    #endif

// Apple devices
#elif __APPLE__
    #pragma message "__APPLE__"
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #pragma message "TARGET_IPHONE_SIMULATOR"
    #elif TARGET_OS_MACCATALYST
        #pragma message "TARGET_OS_MACCATALYST"
    #elif TARGET_OS_IPHONE
        #pragma message "TARGET_OS_IPHONE"
    #elif TARGET_OS_MAC
        #define RC_PLATFORM_MACOS
        #pragma message "TARGET_OS_MAC"
    #else
        #error "Unknown Apple platform"
    #endif

// Linux based
#elif __ANDROID__
    #define RC_PLATFORM_LINUX      
    #pragma message "__ANDROID__"
#elif __linux__
    #define RC_PLATFORM_LINUX      
    #pragma message "__linux__"
#elif __unix__
    #define RC_PLATFORM_LINUX      
    #pragma message "__unix__"
#elif defined(_POSIX_VERSION)
    #define RC_PLATFORM_LINUX      
    #pragma message "_POSIX_VERSION"
#else
    #error "Unknown compiler"
#endif
