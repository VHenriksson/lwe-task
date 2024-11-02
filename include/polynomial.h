#pragma once
#include <vector>
#include <cstdint>
#include <array>
#include <cstddef>

template <uint16_t N>
class Polynomial
{
private:
    std::array<uint32_t, N> coefficients;
public:

    /**
     * @brief Getter method for the coefficients of the polynomial.
     */
const std::array<uint32_t, N> get_coefficients() const { return coefficients; }

/**
     * @brief Addition operator for polynomials.
     * 
     * This function adds two polynomials together.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return Polynomial The result of the addition
     */
    friend Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs)
    {
        auto result_vector = std::array<uint32_t, N>();
        for (size_t i = 0; i < lhs.coefficients.size(); i++) {
            result_vector[i] = lhs.coefficients[i] + rhs.coefficients[i];
        }
        return Polynomial(result_vector);
    }

/**
     * @brief Addition operator for polynomials.
     * 
     * This function adds the rhs polynomial to the lhs.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return Polynomial The result of the addition
     */
    Polynomial operator+=(const Polynomial &rhs)
    {
        for (size_t i = 0; i < coefficients.size(); i++) {
            coefficients[i] += rhs.coefficients[i];
        }
        return *this;
    }

/**
     * @brief Subtraction operator for polynomials.
     * 
     * This function subtracts two polynomials.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return Polynomial The result of the subtraction
     */
    Polynomial operator-=(const Polynomial &rhs)
    {
        for (size_t i = 0; i < coefficients.size(); i++) {
            coefficients[i] -= rhs.coefficients[i];
        }
        return *this;
    }


    /**
     * @brief Multiplication operator for polynomials.
     * 
     * This function multiplies two polynomials together.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return Polynomial The result of the multiplication
     */
    friend Polynomial operator*(const Polynomial &lhs, const Polynomial &rhs)
    {
        std::array<uint32_t, N> result{};
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j <= i; j++) {
                result[i] += lhs.coefficients[j]*rhs.coefficients[i-j];
            }
            for (size_t j = i+1; j < N; j++)
            {
                result[i] -= lhs.coefficients[j]*rhs.coefficients[N-(j-i)];

            }
        }
        return Polynomial(result);
    }

/**
     * @brief Equality operator for polynomials.
     * 
     * Two polynomials are equal if their coefficients are equal.
     * 
     * @param lhs The left hand side
     * @param rhs The right hand side
     * @return true If the polynomials are equal
     * @return false If the polynomials are not equal
     */
    friend bool operator==(const Polynomial &lhs, const Polynomial &rhs)
    {
        for (size_t i = 0; i < N; i++) {
            if (lhs.coefficients[i] != rhs.coefficients[i]) {
                return false;
            }
        }
        return true;
    }
    friend bool operator!=(const Polynomial &lhs, const Polynomial &rhs) { return !(lhs == rhs); }

    /**
     * @brief A polynomial modulo X^N + 1 with coefficients in Z_{2^32} (this approximates
     * R/Z from TFHE).
     * 
     * @param coefficients The coefficients of the polynomial.
     */
    Polynomial(const std::vector<uint32_t>& coefficients)
    {
        auto prel_coefficients = std::array<uint32_t,2*N>();
        for (size_t i = 0; i < coefficients.size(); i++)
        {
            prel_coefficients[i%(2*N)] = coefficients[i];    
        }
        this->coefficients = std::array<uint32_t, N>();
        for (size_t i = 0; i < N; i++)
        {
            this->coefficients[i] = prel_coefficients[i] - prel_coefficients[i+N];
        }
    }

    /**
     * @brief A polynomial modulo X^N + 1 with coefficients in Z_{2^32} (this approximates
     * R/Z from TFHE).
     * 
     * @param coefficients The coefficients of the polynomial.
     */
    Polynomial(const std::array<uint32_t, N>& coefficients): coefficients(coefficients) {}

    /**
     * @brief A polynomial modulo X^N + 1 with coefficients in Z_{2^32} (this approximates
     * R/Z from TFHE).
     */
    Polynomial() = default;
};
