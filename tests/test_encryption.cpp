#include <catch2/catch.hpp>
#include "secret_key.h"

TEST_CASE("Test encryption", "[Encryption]") {

    SECTION("Test same plaintext does not return same ciphertext") {
        auto secret_key = SecretKey<2, 4>();
        std::array<uint32_t, 4> plain = {1, 2, 3, 4};
        auto cipher1 = secret_key.encrypt(plain);
        auto cipher2 = secret_key.encrypt(plain);
        REQUIRE(cipher1 != cipher2);
    }
}

TEST_CASE("Test encrypt decrypt", "[Encryption]") {

    SECTION("Test encryption creates valid decryption") {
        auto secret_key = SecretKey<2, 4>();
        std::array<uint32_t, 4> plain = {1, 2, 3, 4};
        auto cipher = secret_key.encrypt(plain);
        auto decrypted = secret_key.decrypt(cipher);
        REQUIRE(decrypted == plain);
    }
}

TEST_CASE("Test encrypt add decrypt", "[Encryption]") {

    SECTION("Test can add two ciphertexts") {
        auto secret_key = SecretKey<2, 4>();
        std::array<uint32_t, 4> plain_1 = {1, 2, 3, 4};
        std::array<uint32_t, 4> plain_2 = {5, 8, 15, 0};
        std::array<uint32_t, 4> expected_result = {6, 10, 2, 4};
        auto cipher_1 = secret_key.encrypt(plain_1);
        auto cipher_2 = secret_key.encrypt(plain_2);
        auto decrypted = secret_key.decrypt(cipher_1 + cipher_2);
        REQUIRE(decrypted == expected_result);
    }

    SECTION("Test can add l ciphertexts (currently arround 100000)") {
        int l = 100000;
        auto secret_key = SecretKey<2, 4>();
        std::array<uint32_t, 4> plain_1 = {1, 2, 3, 4};
        std::array<uint32_t, 4> plain_2 = {5, 8, 15, 0};
        std::array<uint32_t, 4> expected_result = {6, 10, 2, 4};
        auto cipher_1 = secret_key.encrypt(plain_1);
                for (size_t i = 0; i < l-1; i++)
        {
// We need to create a new cipher text each time
            // in order to have independent errors
            auto cipher_2 = secret_key.encrypt(plain_2);
            // We add multiple times. Testing the limits.
            cipher_1 += cipher_2; 
            // This code creates a reference text
            for (size_t j = 0; j < plain_1.size(); j++)
            {
                plain_1[j] += plain_2[j];
                plain_1[j] %= 16;
            }
            
        }
        auto decrypted = secret_key.decrypt(cipher_1);
        REQUIRE(decrypted == plain_1);
    }
}