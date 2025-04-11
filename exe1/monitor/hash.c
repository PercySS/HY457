#include "hash.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

void compute_hashes(const char *filename, char *md5_str, char *sha256_str) {
    unsigned char md5[MD5_DIGEST_LENGTH];
    unsigned char sha256[SHA256_DIGEST_LENGTH];
    unsigned char buffer[BUF_SIZE];

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    MD5_CTX md5_ctx;
    SHA256_CTX sha256_ctx;
    MD5_Init(&md5_ctx);
    SHA256_Init(&sha256_ctx);

    ssize_t bytes;
    while ((bytes = read(fd, buffer, BUF_SIZE)) > 0) {
        MD5_Update(&md5_ctx, buffer, bytes);
        SHA256_Update(&sha256_ctx, buffer, bytes);
    }

    MD5_Final(md5, &md5_ctx);
    SHA256_Final(sha256, &sha256_ctx);
    close(fd);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf(&md5_str[i * 2], "%02x", md5[i]);
    md5_str[2 * MD5_DIGEST_LENGTH] = '\0';

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&sha256_str[i * 2], "%02x", sha256[i]);
    sha256_str[2 * SHA256_DIGEST_LENGTH] = '\0';
}
