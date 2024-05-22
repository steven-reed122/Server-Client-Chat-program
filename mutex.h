//This file exists because mutliple files all use mutex so they need to use the
//same one. This file instantiates that same one globally for all files.

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

extern pthread_mutex_t lock;

#endif // MUTEX_H

