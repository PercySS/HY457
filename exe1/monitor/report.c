#include "report.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYSCALLS 256
#define MAX_FILES 256
#define MAX_HOSTS 64

typedef struct {
    char name[32];
    int count;
} syscall_entry;

typedef struct {
    char path[256];
    char type[8]; /* "read"/"write" */
} file_access;

static syscall_entry syscall_table[MAX_SYSCALLS];
static int syscall_count = 0;

static file_access file_table[MAX_FILES];
static int file_count = 0;

static char hosts[MAX_HOSTS][256];
static int host_count = 0;

void report_add_syscall(const char *name) {
    for (int i = 0; i < syscall_count; i++) {
        if (strcmp(syscall_table[i].name, name) == 0) {
            syscall_table[i].count++;
            return;
        }
    }
    strncpy(syscall_table[syscall_count].name, name, 31);
    syscall_table[syscall_count].count = 1;
    syscall_count++;
}

void report_add_file_access(const char *path, const char *type) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].path, path) == 0 &&
            strcmp(file_table[i].type, type) == 0) {
            return; /* already exists */ 
        }
    }
    strncpy(file_table[file_count].path, path, 255);
    strncpy(file_table[file_count].type, type, 7);
    file_count++;
}

void report_add_host(const char *host) {
    for (int i = 0; i < host_count; i++) {
        if (strcmp(hosts[i], host) == 0)
            return; /* already exists */
    }
    strncpy(hosts[host_count], host, 255);
    host_count++;
}

void report_generate(void) {
    FILE *fp = fopen("report.txt", "w");
    if (!fp) {
        perror("fopen");
        return;
    }

    fprintf(fp, "===== System Calls =====\n");
    for (int i = 0; i < syscall_count; i++) {
        fprintf(fp, "%s: %d times\n", syscall_table[i].name, syscall_table[i].count);
    }

    fprintf(fp, "\n===== File Accesses =====\n");
    for (int i = 0; i < file_count; i++) {
        fprintf(fp, "[%s] %s\n", file_table[i].type, file_table[i].path);
    }

    fprintf(fp, "\n===== Host Interactions =====\n");
    for (int i = 0; i < host_count; i++) {
        fprintf(fp, "Host: %s\n", hosts[i]);
    }

    fclose(fp);
}
