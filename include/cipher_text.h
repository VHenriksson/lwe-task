#pragma once
#include <vector>
#include <algorithm>
#include "polynomial.h"

template <uint16_t k, uint16_t N>
/**
 * @brief Represents a cipher text.
 * 
 * This class encapsulates a cipher text consisting of an array of polynomials and a single polynomial.
 * It provides comparison operators for equality and inequality, as well as addition operators.
 */
struct CipherText
{
    std::array<Polynomial<N>,k> a;
    Polynomial<N> b;

    /**
     * @brief Provides direct access to the polynomial coefficients of b.
     * 
     * Used for performance reasons, when direct access is needed.
     * 
     */
    uint32_t* b_data() { return b.data(); }

    /**
     * @brief Equality operator for cipher texts.
     * 
     * Two cipher texts are equal if their polynomials are equal.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return true If the cipher texts are equal
     * @return false If the cipher texts are not equal
     */
    friend bool operator==(const CipherText& lhs, const CipherText& rhs)
    {
        return lhs.b == rhs.b && std::equal(lhs.a.begin(), lhs.a.end(), rhs.a.begin());
    }

    /**
     * @brief Inequality operator for cipher texts.
     * 
     * Implemented as a negation of the equality operator.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return true If the cipher texts are not equal
     * @return false If the cipher texts are equal
     */
    friend bool operator!=(const CipherText& lhs, const CipherText& rhs){return !(lhs == rhs);};

    /**
     * @brief Adds two cipher texts together by adding the polynomial coefficients.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return CipherText The result
     */
    friend CipherText operator+(const CipherText& lhs, const CipherText& rhs){
        CipherText<k,N> result = {std::array<Polynomial<N>,k>(), Polynomial<N>(std::vector<uint32_t>{})};
        for (size_t i = 0; i < k; i++) {
            result.a[i] = lhs.a[i] + rhs.a[i];
        }
        result.b = lhs.b + rhs.b;
        return result;
    }

    /**
     * @brief Adds the right hand ciphertext to the left hand side,
     * by adding the polynomial coefficients. 
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return CipherText The result
     */
    friend CipherText operator+=(CipherText& lhs, const CipherText& rhs){
        for (size_t i = 0; i < k; i++) {
            lhs.a[i] += rhs.a[i];
        }
        lhs.b += rhs.b;
        return lhs;
    }
};