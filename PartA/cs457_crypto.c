#include "cs457_crypto.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


char* one_time_pad_encr(const char* plaintext, const char* key, int length) {
    char* ciphertext = (char*)malloc(length + 1);
    if (!ciphertext) return NULL;

    for (int i = 0; i < length; i++) {
        ciphertext[i] = plaintext[i] ^ key[i];
    }

    ciphertext[length] = '\0'; 

    return ciphertext;
}

char* one_time_pad_decr(const char* ciphertext, const char* key, int length) {
    char* plaintext = (char*)malloc(length + 1);

    if (!plaintext) return NULL;

    for (int i = 0; i < length; i++) {
        plaintext[i] = ciphertext[i] ^ key[i];
    }

    plaintext[length] = '\0'; 

    return plaintext;
}

static int affine_encrypt_char(int x) {
    return (3 * x + 8) % 26;
}

static int affine_decrypt_char(int y) {
    return (9 * (y - 8 + 26)) % 26;
}

char* affine_encr(const char* plaintext) {
    int length = strlen(plaintext);
    char* result = (char*)malloc(length + 1);
    if (!result) return NULL;

    for (int i = 0; i < length; i++) {
        char c = plaintext[i];

        if (isupper(c)) {
            result[i] = 'A' + affine_encrypt_char(c - 'A');
        } else if (islower(c)) {
            result[i] = 'a' + affine_encrypt_char(c - 'a');
        } else {
            result[i] = c;
        }
    }

    result[length] = '\0';
    return result;
}

char* affine_decr(const char* ciphertext) {
    int length = strlen(ciphertext);
    char* result = (char*)malloc(length + 1);
    if (!result) return NULL;

    for (int i = 0; i < length; i++) {
        char c = ciphertext[i];

        if (isupper(c)) {
            result[i] = 'A' + affine_decrypt_char(c - 'A');
        } else if (islower(c)) {
            result[i] = 'a' + affine_decrypt_char(c - 'a');
        } else {
            result[i] = c;
        }
    }

    result[length] = '\0';
    return result;
}

char* scytale_encr(const char* plaintext, int diameter) {
    int len = strlen(plaintext);
    char* clean = malloc(len + 1);
    int clen = 0;

    for (int i = 0; i < len; ++i)
        if (isalpha(plaintext[i]))
            clean[clen++] = plaintext[i];

    clean[clen] = '\0';

    int rows = (clen + diameter - 1) / diameter;
    char* result = malloc(clen + 1);
    int k = 0;

    for (int col = 0; col < diameter; ++col)
        for (int row = 0; row < rows; ++row) {
            int idx = row * diameter + col;
            if (idx < clen)
                result[k++] = clean[idx];
        }

    result[k] = '\0';
    free(clean);
    return result;
}

char* scytale_decr(const char* ciphertext, int diameter) {
    int clen = strlen(ciphertext);
    int rows = (clen + diameter - 1) / diameter;

    int full_cols = clen % diameter;
    if (full_cols == 0) full_cols = diameter;

    // Allocate grid [rows][diameter]
    char** grid = malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; ++i)
        grid[i] = malloc(diameter);

    // Fill grid column-wise (only fill when k < clen)
    int k = 0;
    for (int col = 0; col < diameter; ++col) {
        int fill_rows = (col < full_cols) ? rows : rows - 1;
        for (int row = 0; row < fill_rows; ++row) {
            grid[row][col] = ciphertext[k++];
        }
    }

    // Read row-wise into result
    char* result = malloc(clen + 1);
    k = 0;
    for (int row = 0; row < rows; ++row)
        for (int col = 0; col < diameter; ++col)
            if (row * diameter + col < clen && grid[row][col])
                result[k++] = grid[row][col];

    result[k] = '\0';

    for (int i = 0; i < rows; ++i)
        free(grid[i]);
    free(grid);

    return result;
}


char* reinsert_format(const char* stripped, const char* original) {
    int o_len = strlen(original);
    int s_len = strlen(stripped);
    char* restored = malloc(o_len + 1);
    int si = 0;

    for (int i = 0; i < o_len; ++i) {
        if (isalpha(original[i])) {
            if (si < s_len) {
                restored[i] = isupper(original[i]) ? toupper(stripped[si]) : tolower(stripped[si]);
                si++;
            } else {
                restored[i] = ' '; // padding if something went wrong
            }
        } else {
            restored[i] = original[i];  // keep punctuation, spacing, etc.
        }
    }

    restored[o_len] = '\0';
    return restored;
}

