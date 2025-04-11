#include "syscall_table.h"
#include <stddef.h>

typedef struct {
    long number;
    const char *name;
} syscall_entry;

static syscall_entry syscall_table[] = {
    {0, "read"},
    {1, "write"},
    {2, "open"},
    {3, "close"},
    {5, "fstat"},
    {9, "mmap"},
    {10, "mprotect"},
    {11, "munmap"},
    {12, "brk"},
    {21, "access"},
    {32, "dup"},
    {33, "dup2"},
    {39, "getpid"},
    {41, "socket"},
    {42, "connect"},
    {44, "sendto"},
    {45, "recvfrom"},
    {59, "execve"},
    {60, "exit"},
    {61, "wait4"},
    {62, "kill"},
    {63, "uname"},
    {72, "fcntl"},
    {89, "readlink"},
    {97, "getrlimit"},
    {158, "arch_prctl"},
    {231, "exit_group"},
    {257, "openat"},
    {263, "unlinkat"},
    { -1, NULL } // Τερματισμός πίνακα
};

const char* get_syscall_name(long syscall_num) {
    for (int i = 0; syscall_table[i].name != NULL; i++) {
        if (syscall_table[i].number == syscall_num)
            return syscall_table[i].name;
    }
    return "unknown";
}
