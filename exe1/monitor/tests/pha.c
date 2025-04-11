
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>     // socket(), sendto()
#include <netinet/in.h>     // AF_INET, sockaddr_in
#include <arpa/inet.h>      

int main() {
    printf("PHA Test starting...\n");

    int fd = open("/etc/hostname", O_RDONLY);
    if (fd >= 0) {
        char buffer[64];
        read(fd, buffer, 63);
        buffer[63] = '\0';
        printf("Read from file: %s\n", buffer);
        close(fd);
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd >= 0) {
        const char *msg = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
        sendto(sockfd, msg, strlen(msg), 0, NULL, 0);
        close(sockfd);
    }

    return 0;
}
