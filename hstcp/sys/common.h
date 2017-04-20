/*
 * common.h
 *
 *  Created on: 2017��4��13��
 *      Author: hlf
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <sys/socket.h>
#include <dlfcn.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
#include <fnmatch.h>
#include <limits.h>
#include <ctype.h>
#include <sys/poll.h>
#include <netipx/ipx.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <new>
#include <error.h>
#include <syslog.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <signal.h>
#ifndef MAX_PATH
    #define MAX_PATH			256
#endif

static const int INFINITE = (-1);

#define FBASE_NEW new(std::nothrow)
#define NEW  new(std::nothrow)


#endif /* COMMON_H_ */
