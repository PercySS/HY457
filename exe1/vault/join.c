#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "join.h"

void solve(int x1, int y1, int x2, int y2, int x3, int y3, int *a, int *b, int *c) {
    int det = (x1 - x2) * (x1 - x3) * (x2 - x3);

    *a = (y1 * (x2 - x3) - y2 * (x1 - x3) + y3 * (x1 - x2)) / det;
    *b = (y1 * (x3 * x3 - x2 * x2) - y2 * (x3 * x3 - x1 * x1) + y3 * (x2 * x2 - x1 * x1)) / det;
    *c = (y1 * (x2 * x3 * (x2 - x3)) - y2 * (x1 * x3 * (x1 - x3)) + y3 * (x1 * x2 * (x1 - x2))) / det;
}

void join() {
    FILE *fp = fopen("points.txt", "r");
    if (!fp) {
        perror("points.txt");
        return;
    }

    int points[10][2], count = 0;
    while (fscanf(fp, "%d %d", &points[count][0], &points[count][1]) == 2 && count < 10) {
        count++;
    }
    fclose(fp);

    srand(time(NULL));
    int idx1 = rand() % 10;
    int idx2, idx3;
    do { idx2 = rand() % 10; } while (idx2 == idx1);
    do { idx3 = rand() % 10; } while (idx3 == idx1 || idx3 == idx2);

    int x1 = points[idx1][0], y1 = points[idx1][1];
    int x2 = points[idx2][0], y2 = points[idx2][1];
    int x3 = points[idx3][0], y3 = points[idx3][1];

    printf("Share count is 3\n");
    printf("f(%d) = %d\n", x1, y1);
    printf("f(%d) = %d\n", x2, y2);
    printf("f(%d) = %d\n", x3, y3);

    int a, b, password;
    solve(x1, y1, x2, y2, x3, y3, &a, &b, &password);

    printf("Computed that:\n");
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("Password is: %d\n", password);
}
