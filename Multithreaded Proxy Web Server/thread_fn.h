#ifndef THREAD_FN_H
#define THREAD_FN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "ParsedRequest.h"
#include "find.h"
#include "handle_request.h"
#include "send_error_message.h"

void *thread_fn(void *socketNew);

#endif // THREAD_FN_H
