#pragma once
// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#    pragma message("WIN32 || _WIN32 || __WIN32__ || __NT__")
#    ifndef RC_PLATFORM_WINDOWS
#        define RC_PLATFORM_WINDOWS
#    endif
#    ifdef _WIN64
#    else
#        pragma message("RC_NOT_WINDOWS")
#    endif

// Apple devices
#elif __APPLE__
#    include <TargetConditionals.h>
#    if TARGET_IPHONE_SIMULATOR
#    elif TARGET_OS_MACCATALYST
#    elif TARGET_OS_IPHONE
#    elif TARGET_OS_MAC
#        ifndef RC_PLATFORM_MACOS
#            define RC_PLATFORM_MACOS
#        endif
#    else
#        error "Unknown Apple platform"
#    endif

// Linux based
#elif __ANDROID__
#    ifndef RC_PLATFORM_LINUX
#        define RC_PLATFORM_LINUX
#    endif
#elif __linux__
#    ifndef RC_PLATFORM_LINUX
#        define RC_PLATFORM_LINUX
#    endif
#elif __unix__
#    ifndef RC_PLATFORM_LINUX
#        define RC_PLATFORM_LINUX
#    endif
#elif defined(_POSIX_VERSION)
#    ifndef RC_PLATFORM_LINUX
#        define RC_PLATFORM_LINUX
#    endif
#else
#    error("Unknown compiler")
#endif
