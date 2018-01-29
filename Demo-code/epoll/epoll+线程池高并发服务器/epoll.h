#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <assert.h>
#include <pthread.h>
#include "pool.h"
#include "read_client.h"

void *read_thread(void *argv);

#endif
