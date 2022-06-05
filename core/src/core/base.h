#pragma once

#if defined(STATS_DEBUG)
#	if defined(STATS_WINDOWS)
#		define STATS_DEBUGBREAK() __debugbreak()
#	elif defined(STATS_LINUX)
#		include <signal.h>
#		define STATS_DEBUGBREAK() raise(SIGTRAP)
#	else
#		error "Platform does not suported debug yet!"
#	endif
#	define STATS_ENABLE_ASSERTS
#else
#	define STATS_DEBUGBREAK()
#endif

#define STATS_EXPAND_MACRO(x) x
#define STATS_STRINGIFY_MACRO(x) #x

#include "Networking/net.h"
#include "log.h"
