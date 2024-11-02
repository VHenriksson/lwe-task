#include "cipher_text.h"
#include "polynomial.h"
#include "secret_key.h"
#include <array>
#include <numeric>



int main() {
    std::array<uint32_t, 1024> plain;
    std::iota(plain.begin(), plain.end(), 0);
    SecretKey<1, 1024> secret_key;
    CipherText<1, 1024> cipher;
    std::array<uint32_t, 1024> decrypted;
    // Currently takes 18 seconds
    for (int i = 0; i < 10000; i++)
    {
        cipher = secret_key.encrypt(plain);
        decrypted = secret_key.decrypt(cipher);
    }
    return 0;
}