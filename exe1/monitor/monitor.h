#ifndef MONITOR_H
#define MONITOR_H

#define _GNU_SOURCE

/* Standard Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

/* System Programming */
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <dirent.h>

// Crypto Libraries
#include <openssl/md5.h>
#include <openssl/sha.h>

#define BUF_SIZE 4096


#endif
