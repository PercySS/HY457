#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "split.h"

void split(int password) {
    srand(time(NULL));
    int a = rand() % 10 + 1;
    int b = rand() % 10 + 1;

    FILE *fp = fopen("points.txt", "w");
    FILE *pw = fopen("password.txt", "w");

    fprintf(pw, "%d\n", password);
    fclose(pw);

    printf("Randomly selected a = %d, b = %d\n", a, b);
    for (int x = 1; x <= 10; x++) {
        int y = a * x * x + b * x + password;
        fprintf(fp, "%d %d\n", x, y);
        printf("Point (%d,%d)\n", x, y);
    }

    fclose(fp);
}
