#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100000
#define DICTIONARY "words.txt"
#define CIPHERTEXT "ciphertext.txt"

static float res[26][3];

void letter_freq(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: File not found\n");
        return;
    }
    int c;
    int freq[26] = {0}, total = 0;
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {
            freq[tolower(c) - 'a']++;
            total++;
        }
    }
    fclose(file);

    for (int i = 0; i < 26; i++) {
        res[i][0] = freq[i];
        res[i][1] = (float)freq[i] / total;
        res[i][2] = i;
    }

    for (int i = 0; i < 26; i++) {
        for (int j = i + 1; j < 26; j++) {
            if (res[i][0] < res[j][0]) {
                for (int k = 0; k < 3; k++) {
                    float tmp = res[i][k];
                    res[i][k] = res[j][k];
                    res[j][k] = tmp;
                }
            }
        }
    }

    printf("Letter frequencies in %s:\n", filename);
    for (int i = 0; i < 26; i++) {
        printf("%c: %.2f%%\t", (int)res[i][2] + 'A', res[i][1] * 100);
        if ((i + 1) % 6 == 0) printf("\n");
    }
    printf("\n");
}

char* matching_words(char* word, char* filename) {
    char* suggestion = malloc(100000);
    if (!suggestion) return NULL;
    suggestion[0] = '\0';

    int len = strlen(word);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: File not found\n");
        return NULL;
    }

    char temp[100];
    int max = 0;
    while (fscanf(file, "%s", temp) != EOF) {
        if ((int)strlen(temp) != len) continue;
        int score = 0;
        for (int i = 0; i < len; i++) {
            if (word[i] != '*' && tolower(temp[i]) != tolower(word[i])) {
                score = 0;
                break;
            }
            if (tolower(temp[i]) == tolower(word[i])) score++;
        }
        if (score > max) max = score;
    }

    rewind(file);
    while (fscanf(file, "%s", temp) != EOF) {
        if ((int)strlen(temp) != len) continue;
        int score = 0;
        for (int i = 0; i < len; i++) {
            if (word[i] != '*' && tolower(temp[i]) != tolower(word[i])) {
                score = 0;
                break;
            }
            if (tolower(temp[i]) == tolower(word[i])) score++;
        }
        if (score == max) {
            strcat(suggestion, temp);
            strcat(suggestion, "\t");
        }
    }
    fclose(file);
    return suggestion;
}

void decryptor(char* ciphertext, char* ciphertext_file) {
    int len = strlen(ciphertext);
    char* partially_decrypted = malloc(len + 1);
    char map[27];
    char new_map[27];
    strcpy(map, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    strcpy(new_map, "**************************");

    for (int i = 0; i < len; i++) {
        if (islower(ciphertext[i])) ciphertext[i] = toupper(ciphertext[i]);
    }

    printf("English ");
    letter_freq(DICTIONARY);
    printf("Ciphertext ");
    letter_freq(CIPHERTEXT);
    getchar();

    int c = 0;
    while (c != EOF) {
        char m, n_m;
        printf("\nNext mapping: ");
        m = getchar();
        if (!isupper(m)) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        printf("%c -> ", m);
        getchar();
        n_m = getchar();
        if (!isupper(n_m)) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        new_map[m - 'A'] = n_m;
        for (int i = 0; i < len; i++) {
            if (isupper(ciphertext[i]))
                partially_decrypted[i] = new_map[ciphertext[i] - 'A'];
            else
                partially_decrypted[i] = ciphertext[i];
        }
        partially_decrypted[len] = '\0';
        printf("\nPartially decrypted text:\n%s\n", partially_decrypted);

        printf("Enter partially decrypted word: ");
        char word[100];
        scanf("%s", word);
        char* suggestion = matching_words(word, DICTIONARY);
        printf("Suggestion: %s\n", suggestion);
        free(suggestion);

        printf("\nPress any key to continue or Ctrl-D to stop\n");
        getchar();
        c = getchar();
    }
    free(partially_decrypted);
}

int main() {
    int choice;
    do {
        printf("\n===== Substitution Cipher Tool =====\n");
        printf("1. Show letter frequencies\n");
        printf("2. Suggest matching words\n");
        printf("3. Manual decryptor\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            char file[100];
            letter_freq("words.txt");
            letter_freq("ciphertext.txt");
        }
        else if (choice == 2) {
            char pattern[100];
            printf("Enter word pattern (e.g. th**): ");
            scanf("%s", pattern);
            char* sugg = matching_words(pattern, DICTIONARY);
            printf("Suggestions: %s\n", sugg);
            free(sugg);
        }
        else if (choice == 3) {
            char filename[100];
            FILE* file = fopen(CIPHERTEXT, "r");
            if (!file) {
                printf("Error: File not found\n");
                continue;
            }
            fseek(file, 0, SEEK_END);
            long size = ftell(file);
            rewind(file);
            char* buffer = malloc(size + 1);
            fread(buffer, 1, size, file);
            fclose(file);
            buffer[size] = '\0';
            decryptor(buffer, filename);
            free(buffer);
        }
        else if (choice != 0) {
            printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
