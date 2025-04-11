#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    printf("PHA2: writing to a file...\n");

    
    int fd = open("pha2_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0) {
        const char *msg = "This is a test from pha2.\n";
        write(fd, msg, strlen(msg));
        close(fd);
    }

    
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd >= 0) {
        const char *http = "POST /upload HTTP/1.1\r\nHost: upload.test\r\n\r\n";
        sendto(sockfd, http, strlen(http), 0, NULL, 0);
        close(sockfd);
    }

    return 0;
}
