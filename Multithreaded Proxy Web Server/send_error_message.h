#ifndef SEND_ERROR_MESSAGE_H
#define SEND_ERROR_MESSAGE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int sendErrorMessage(int socket, int status_code);

#endif // SEND_ERROR_MESSAGE_H
