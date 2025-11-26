#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "cs457_crypto.h"


#define MAX_LENGTH 1024


void get_random_key(char* key, int length) {
    int fd = open("/dev/urandom", O_RDONLY);

    if (fd < 0) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }

    if (read(fd, key, length) != length) {
        perror("Failed to read random bytes");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}


int main() {
    printf("========== One Time Pad ==========\n");
    char input[MAX_LENGTH];

    printf("Enter the plaintext: ");
    fgets(input, MAX_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0;

    int length = strlen(input);
    char* key = (char*)malloc(length + 1);
    get_random_key(key, length);


    char* cipher = one_time_pad_encr(input, key, length);
    char* decrypted = one_time_pad_decr(cipher, key, length);

    printf("Ciphertext: %s\n", cipher);

    for (int i = 0; i < length; i++) {
        printf("%02x ", (unsigned char)cipher[i]);
    }
    printf("\n");

    printf("Decrypted text: %s\n", decrypted);

    free(key);
    free(cipher);
    free(decrypted);

    printf("========== Affine ==========\n");
    char affine_input[MAX_LENGTH];
    printf("Enter text for Affine Cipher: ");
    fgets(affine_input, MAX_LENGTH, stdin);
    affine_input[strcspn(affine_input, "\n")] = '\0';

    char* aff_encr = affine_encr(affine_input);
    char* aff_decr = affine_decr(aff_encr);

    printf("Affine Encrypted: %s\n", aff_encr);
    printf("Affine Decrypted: %s\n", aff_decr);

    free(aff_encr);
    free(aff_decr);
    

    printf("========== Scytale ==========\n");
    char scy_input[MAX_LENGTH];
    int diameter;

    printf("\nEnter text for Scytale Cipher: ");
    fgets(scy_input, MAX_LENGTH, stdin);
    scy_input[strcspn(scy_input, "\n")] = '\0';

    printf("Enter rod diameter: ");
    scanf("%d", &diameter);
    getchar();
    char* scy_encr = scytale_encr(scy_input, diameter);
    char* scy_decr_raw = scytale_decr(scy_encr, diameter);
    char* scy_decr_final = reinsert_format(scy_decr_raw, scy_input);

    printf("Scytale Encrypted: %s\n", scy_encr);
    printf("Scytale Decrypted: %s\n", scy_decr_final);

    free(scy_encr);
    free(scy_decr_raw);
    free(scy_decr_final);
    return 0;
}