#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD 256
#define HASH_COUNT 3

const char* hashes[HASH_COUNT] = {
    "d077f244def8a70e5ea758bd8352fcd8",
    "df53ca268240ca76670c8566ee54568a",
    "6b718641741f992e68ec3712718561b8"
};

const char* hash_names[HASH_COUNT] = {
    "Hash 1", "Hash 2", "Hash 3"
};

const char* wordlist_path = "rockyou.txt";

// ===== Utility =====

char* hash_md5(const char* input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((const unsigned char*)input, strlen(input), digest);

    char* md5_string = malloc(33);
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        sprintf(&md5_string[i*2], "%02x", digest[i]);

    return md5_string;
}

int hashes_match(const char* hash1, const char* hash2) {
    return strcasecmp(hash1, hash2) == 0;
}

// ===== Dictionary Attack =====

void dictionary_attack(const char* target_hash) {
    FILE* file = fopen(wordlist_path, "r");
    if (!file) {
        perror("rockyou.txt");
        return;
    }

    char word[MAX_WORD];
    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = 0;
        if (strlen(word) == 0) continue;

        char* attempt = hash_md5(word);
        if (hashes_match(attempt, target_hash)) {
            printf("Found (dictionary): %s\n", word);
            free(attempt);
            fclose(file);
            return;
        }
        free(attempt);
    }

    fclose(file);
    printf("Not found in dictionary.\n");
}

// ===== Brute Force Attack =====

char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
#define CHARSET_SIZE (sizeof(charset) - 1)

int done = 0;

void brute_force_recursive(char* buffer, int depth, int max_depth, const char* target_hash) {
    if (done) return;
    if (depth == max_depth) {
        buffer[depth] = '\0';
        char* h = hash_md5(buffer);
        if (hashes_match(h, target_hash)) {
            printf("Found (brute): %s\n", buffer);
            done = 1;
        }
        free(h);
        return;
    }

    for (int i = 0; i < CHARSET_SIZE; ++i) {
        buffer[depth] = charset[i];
        brute_force_recursive(buffer, depth + 1, max_depth, target_hash);
        if (done) break;
    }
}

void brute_force_attack(const char* target_hash) {
    char buffer[9];
    done = 0;

    for (int len = 1; len <= 8; ++len) {
        brute_force_recursive(buffer, 0, len, target_hash);
        if (done) return;
    }

    printf("Not found by brute-force.\n");
}

// ===== Timed Comparison =====

void time_diff(const char* label, void (*func)(const char*), const char* hash) {
    clock_t start = clock();
    func(hash);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%s time: %.2f seconds\n\n", label, time_spent);
}

int main() {
    printf("MD5 Cracker (Dictionary + Brute Force)\n");

    for (int i = 0; i < HASH_COUNT; ++i) {
        printf("\n============================\n");
        printf("Testing %s: %s\n", hash_names[i], hashes[i]);
        printf("----------------------------\n");

        time_diff("Dictionary attack", dictionary_attack, hashes[i]);
        time_diff("Brute-force attack", brute_force_attack, hashes[i]);
    }

    return 0;
}
