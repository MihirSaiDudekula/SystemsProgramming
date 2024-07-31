#ifndef FIND_H
#define FIND_H

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "cache.h"

struct cache* find(char* url);

#endif // FIND_H
