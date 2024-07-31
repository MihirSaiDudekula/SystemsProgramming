#ifndef ADD_TO_CACHE_H
#define ADD_TO_CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "cache.h"

int add_to_cache(char* data, int size, char* url);

#endif // ADD_TO_CACHE_H
