//This file exists because mutliple files all use mutex so they need to use the
//same one. This file instantiats that same one.

#include "mutex.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

