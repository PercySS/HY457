#ifndef CS457_CRYPTO_H
#define CS457_CRYPTO_H

char* one_time_pad_encr(const char* plaintext, const char* key, int length);
char* one_time_pad_decr(const char* ciphertext, const char* key, int length);


char* affine_encr(const char* plaintext);
char* affine_decr(const char* ciphertext);

char* scytale_encr(const char* plaintext, int diameter);
char* scytale_decr(const char* ciphertext, int diameter);
char* reinsert_format(const char* stripped, const char* original);
#endif // CS457_CRYPTO_H