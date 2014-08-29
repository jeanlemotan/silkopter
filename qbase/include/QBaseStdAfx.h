#include "Platform.h"

#define _USE_MATH_DEFINES
#if defined Q_AVR
#	include <stdint.h>
#	include <math.h>
#	include <stdarg.h>
#	include <vector>
#else
#	include <cstdint>
#	include <cmath>
#	include <cstdarg>
#	include <map>
#	include <vector>
#	include <memory>
#	include <deque>
#	include <functional>
#	include <chrono>
#	include <thread>
#	include <mutex>
#	include <condition_variable>
#	include <boost/signals2.hpp>
#endif


