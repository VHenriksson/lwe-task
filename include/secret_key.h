#pragma once
#include <cstdint>
#include <algorithm>
#include <random>
#include "polynomial.h"
#include "cipher_text.h"

template <uint16_t k, uint16_t N>
/**
 * @brief Represents a secret key.
 * 
 * This class encapsulates a secret key consisting of an array of polynomials.
 * It provides methods for encrypting and decrypting messages.
 */
class SecretKey
{
public:
    SecretKey()
    {
        for (size_t i = 0; i < k; i++)
        {
            std::array<uint32_t, N> coefficients;
            std::generate(coefficients.begin(), coefficients.end(), [this]() { return rd() % 2; });
            key[i] = Polynomial<N>(coefficients);
        }
    }

    /**
     * @brief Encrypts a given plaintext message.
     *
     * This function takes a plaintext which is an array of
     * numbers between 0 and 255 (i.e. a byte), each representing
     * a message and encrypts it using the secret key. To make the
     * implementation easier, the numbers are assumed to be encoded
     * as 32-bit integers, even though they only use 8 bits. They
     * should be in the least significant bits of the 32-bit integers.
     *
     * @param plaintext The plaintext message to be encrypted.
     * @return CipherText The encrypted message.
     */
    CipherText<k, N> encrypt(std::array<uint32_t, N> plain)
    {
        CipherText<k,N> cipher = {std::array<Polynomial<N>,k>(), Polynomial<N>(std::vector<uint32_t>{})};
        for (size_t i = 0; i < k; i++)
        {
            std::vector<uint32_t> coefficients(N);
            std::generate(coefficients.begin(), coefficients.end(), [this]() { return rd(); });
            cipher.a[i] = Polynomial<N>(coefficients);
            cipher.b += key[i] * cipher.a[i];
        }
        // Using the error associated with TFHE1024
        // See e.g. the Lattice Estimator
        // https://github.com/malb/lattice-estimator
        std::normal_distribution<double> d(0, 1 << 7);
        for (size_t i = 0; i < N; i++)
        {
            plain[i] <<= 24;
            int32_t rounded_error = std::round(d(rd));
            // We use unsigned integers, since their overflow is well defined.
            // In practice, $-m$ (as a signed integer) is implemented the same
            // as the inverse of $m$ in Z_{2^32} (as an unsigned integer).
            // Thus, it makes sense to consider unsigned errors. They will act
            // the same as signed errors on signed integers, only with better
            // overflow behaviour.
            uint32_t unsigned_error = static_cast<uint32_t>(rounded_error);
            plain[i] += unsigned_error;
        }
        cipher.b += Polynomial<N>(plain);
        return cipher;
    }

    /**
     * @brief Decrypts a given ciphertext message.
     *
     * This function takes a CipherText object and decrypts it
     * using the secret key.
     *
     * @param plaintext The ciphertext message to be decrypted.
     * @return PlainText The decrypted message.
     */
    std::array<uint32_t,N> decrypt(CipherText<k, N> cipher)
    {
        Polynomial<N> plain_polynomial  = cipher.b;
        for (size_t i = 0; i < key.size(); i++)
        {
            plain_polynomial -= key[i] * cipher.a[i];
        }
        std::array<uint32_t, N> plain_with_error = plain_polynomial.get_coefficients();
        std::array<uint32_t, N> plain;
        for (size_t i = 0; i < N; i++)
        {
            // We center the value inside its interval
            plain_with_error[i] += (1<<23);
            // Now, rounding is easy
            plain[i] = plain_with_error[i] >> 24;
        }
        return plain;
    }



private:
    // This is not generally cryptographically secure, since the random device depends on the underlying system.
    // One should really use e.g. OpenSSL for this, but I have avoided it in order to keep the code self-contained.
    // (Of course, even OpenSSL may be a bad idea if the underlying system is unable to generate enough entropy.)
    std::random_device rd;
    std::array<Polynomial<N>,k> key;

};