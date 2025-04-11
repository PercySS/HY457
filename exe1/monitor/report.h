#ifndef REPORT_H
#define REPORT_H

#include <stdbool.h>


void report_add_syscall(const char *name);


void report_add_file_access(const char *path, const char *type);


void report_add_host(const char *host);


void report_generate(void);

#endif
