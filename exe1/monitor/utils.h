#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

/*  prefix with timestamp */
void print_time_prefix(void);


/* /proc/1234/fd/3 -> /etc/passwd */
char* resolve_fd_path(pid_t pid, int fd);

#endif
