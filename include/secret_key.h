#pragma once
#include <cstdint>
#include <algorithm>
#include <vector>
#include <random>
#include <iostream>
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
     * numbers between 0 and 15, each representing a message
     * and encrypts it using the secret key. Encoding
     * as a uint32_t is only to make the implementation easier.
     *
     * @param plaintext The plaintext message to be encrypted.
     * @return CipherText The encrypted message.
     */
    CipherText<k, N> encrypt(std::array<uint32_t, N> plain)
    {
        CipherText<k,N> cipher = {std::array<Polynomial<N>,k>(), Polynomial<N>(std::vector<uint32_t>{})};
        for (size_t i = 0; i < key.size(); i++)
        {
            std::vector<uint32_t> coefficients(N);
            std::generate(coefficients.begin(), coefficients.end(), [this]() { return rd(); });
            cipher.a[i] = Polynomial<N>(coefficients);
            cipher.b += key[i] * cipher.a[i];
        }
        std::normal_distribution<double> d(0, 1 << 25);
        for (size_t i = 0; i < N; i++)
        {
            plain[i] <<= 28;
            int32_t rounded_error = std::round(d(rd));
            uint32_t unsigned_error = static_cast<uint32_t>(rounded_error);
            plain[i] += unsigned_error;
        }
        cipher.b += Polynomial<N>(plain);
        return cipher;
    }

    static uint32_t prepare_plaintext(uint32_t plain)
    {
        plain <<= 28;
        int32_t rounded_error = std::round(error_distribution(rd));
        uint32_t unsigned_error = static_cast<uint32_t>(rounded_error);
        plain += unsigned_error;
        return plain;
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
            plain_with_error[i] += (1<<27);
            plain[i] = plain_with_error[i] >> 28;
        }
        return plain;
    }



private:
    // This is not generally crpytographically secure, since the random device depends on the underlying system.
    // One should really use e.g. OpenSSL for this, but I have avoided it in order to keep the code self-contained.
    // (Of course, even OpenSSL may be a bad idea if the underlying system is unable to generate enough entropy.)
    std::random_device rd;
    std::array<Polynomial<N>,k> key;

};