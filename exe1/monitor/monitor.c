#include "monitor.h"
#include "hash.h"
#include "report.h"
#include "utils.h"
#include "syscall_table.h"

#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef __x86_64__
#define SYSCALL_NUM(regs) ((regs).orig_rax)
#define ARG1(regs) ((regs).rdi)
#define ARG2(regs) ((regs).rsi)
#define ARG3(regs) ((regs).rdx)
#else
#error "Only x86_64 supported"
#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PHA executable>\n", argv[0]);
        return 1;
    }

    const char *target = argv[1];
    char md5[2 * MD5_DIGEST_LENGTH + 1] = {0};
    char sha256[2 * SHA256_DIGEST_LENGTH + 1] = {0};

    compute_hashes(target, md5, sha256);

    print_time_prefix(); printf("Application Started with argument ‘%s’\n", target);
    print_time_prefix(); printf("MD5 hash:    %s\n", md5);
    print_time_prefix(); printf("SHA256 hash: %s\n", sha256);
    print_time_prefix(); printf("Initialized data structures\n");

    pid_t child = fork();

    if (child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl(target, target, NULL);
        perror("execl");
        exit(1);
    } else if (child > 0) {
        int status;
        waitpid(child, &status, 0);
        print_time_prefix(); printf("Running...\n");

        while (WIFSTOPPED(status)) {
            struct user_regs_struct regs;

            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            waitpid(child, &status, 0);
            if (WIFEXITED(status)) break;

            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            long syscall = SYSCALL_NUM(regs);
            const char *name = get_syscall_name(syscall);

            report_add_syscall(name);

            // --- SENDTO HANDLING ---
            if (strcmp(name, "sendto") == 0) {
                unsigned long addr = ARG2(regs);
                unsigned long len  = ARG3(regs);

                if (len > 0 && len < 1024) {
                    char buffer[1025] = {0};

                    for (unsigned long i = 0; i < len; i += sizeof(long)) {
                        long data = ptrace(PTRACE_PEEKDATA, child, addr + i, NULL);
                        if (data == -1 && errno != 0)
                            break;
                        memcpy(buffer + i, &data, sizeof(long));
                    }

                    if (strncmp(buffer, "GET ", 4) == 0 || strncmp(buffer, "POST ", 5) == 0) {
                        char *host_ptr = strcasestr(buffer, "Host:");
                        if (host_ptr) {
                            host_ptr += 5;
                            while (*host_ptr == ' ') host_ptr++;
                            char *end = strpbrk(host_ptr, "\r\n");
                            if (end) *end = '\0';

                            print_time_prefix();
                            printf("Subprocess interacted with host '%s'\n", host_ptr);

                            report_add_host(host_ptr);
                        }
                    }
                }
            }

            if (strcmp(name, "read") == 0 || strcmp(name, "write") == 0) {
                int fd = ARG1(regs);
                char *path = resolve_fd_path(child, fd);
            
                if (strcmp(path, "unknown") != 0) {
                    report_add_file_access(path, strcmp(name, "read") == 0 ? "read" : "write");
            
                    print_time_prefix();
                    printf("Subprocess accessed file '%s' with %s\n", path, name);
                }
            }

            print_time_prefix();
            printf("Subprocess called '%s' (syscall #%ld)\n", name, syscall);
        }

        print_time_prefix(); printf("Subprocess exited\n");

        report_generate();
        print_time_prefix(); printf("Stored report to ‘report.txt’\n");

    } else {
        perror("fork");
        return 1;
    }

    return 0;
}
