#include <stdio.h>

// ---------- Utilities ----------

int gcd(int a, int b) {
    while (b) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Extended Euclidean Algorithm for modular inverse
int modinv(int e, int phi) {
    int t = 0, newt = 1;
    int r = phi, newr = e;

    while (newr != 0) {
        int quotient = r / newr;
        int temp;

        temp = newt;
        newt = t - quotient * newt;
        t = temp;

        temp = newr;
        newr = r - quotient * newr;
        r = temp;
    }

    if (r > 1) return -1;
    if (t < 0) t += phi;
    return t;
}

// Modular exponentiation (m^e % n)
int modexp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1; // divide by 2
        base = (base * base) % mod;
    }
    return result;
}

// ---------- RSA Encryption ----------

int rsa_encrypt(int m, int e, int n) {
    return modexp(m, e, n);
}

int rsa_decrypt(int c, int d, int n) {
    return modexp(c, d, n);
}

// ---------- Main ----------

int main() {
    int p = 61, q = 53;
    int n = p * q;              // 3233
    int phi = (p - 1) * (q - 1); // 3120
    int e = 65537;

    if (gcd(e, phi) != 1) {
        printf("e is not coprime with phi(n)\n");
        return 1;
    }

    int d = modinv(e, phi);
    if (d == -1) {
        printf("Failed to find modular inverse.\n");
        return 1;
    }

    printf("Public key:  (e = %d, n = %d)\n", e, n);
    printf("Private key: (d = %d, n = %d)\n", d, n);

    // Encrypt message
    int message;
    printf("Enter numeric message (as integer < %d): ", n);
    scanf("%d", &message);

    int cipher = rsa_encrypt(message, e, n);
    printf("Encrypted: %d\n", cipher);

    int decrypted = rsa_decrypt(cipher, d, n);
    printf("Decrypted: %d\n", decrypted);

    return 0;
}
