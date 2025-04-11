#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

void print_time_prefix(void) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%d-%b-%y %H:%M:%S", t);
    printf("[INFO] [%s] ", buf);
}

char* resolve_fd_path(pid_t pid, int fd) {
    char fd_path[PATH_MAX];
    static char resolved[PATH_MAX]; /* static in order to not get lost when return */

    snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd/%d", pid, fd);
    ssize_t len = readlink(fd_path, resolved, sizeof(resolved) - 1);
    
    if (len == -1) {
        strcpy(resolved, "unknown");
    } else {
        resolved[len] = '\0';
    }

    return resolved;
}
