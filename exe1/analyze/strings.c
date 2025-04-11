#include "strings.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MIN_STRING_LEN 4
#define MAX_STRING_LEN 1024

void extract_strings(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        return;
    }

    print_time_prefix();
    printf("Extracting printable strings:\n");

    int c;
    char buffer[MAX_STRING_LEN];
    int len = 0;
    int total = 0;

    while ((c = fgetc(fp)) != EOF) {
        if (isprint(c)) {
            if (len < MAX_STRING_LEN - 1) {
                buffer[len++] = (char)c;
            }
        } else {
            if (len >= MIN_STRING_LEN) {
                buffer[len] = '\0';
                printf("  %s\n", buffer);
                total++;
            }
            len = 0;
        }
    }

    /* Check the end */
    if (len >= MIN_STRING_LEN) {
        buffer[len] = '\0';
        printf("  %s\n", buffer);
        total++;
    }

    fclose(fp);

    print_time_prefix();
    printf("Total strings: %d\n", total);
}
