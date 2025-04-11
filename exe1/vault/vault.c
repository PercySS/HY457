#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "split.h"
#include "join.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <split password | join>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "split") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s split <password>\n", argv[0]);
            return 1;
        }
        int password = atoi(argv[2]);
        split(password);
    } else if (strcmp(argv[1], "join") == 0) {
        join();
    } else {
        fprintf(stderr, "Invalid mode. Use 'split' or 'join'.\n");
        return 1;
    }

    return 0;
}
