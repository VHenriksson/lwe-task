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
    std::array<uint32_t, N> get_coefficients() const { return coefficients; }

    friend Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs)
    {
        auto result_vector = std::array<uint32_t, N>();
        for (size_t i = 0; i < lhs.coefficients.size(); i++) {
            result_vector[i] = lhs.coefficients[i] + rhs.coefficients[i];
        }
        return Polynomial(result_vector);
    }

    Polynomial operator+=(const Polynomial &rhs)
    {
        for (size_t i = 0; i < coefficients.size(); i++) {
            coefficients[i] += rhs.coefficients[i];
        }
        return *this;
    }

    Polynomial operator-=(const Polynomial &rhs)
    {
        for (size_t i = 0; i < coefficients.size(); i++) {
            coefficients[i] -= rhs.coefficients[i];
        }
        return *this;
    }


    // This should be implemented using (hardware accelerated :D ) NTT. However,
    // in the limited time for the exercise, I was unable to understand how
    // to find primitive 1024-roots of unity in Z_{2^32}.
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
    Polynomial(const std::array<uint32_t, N>& coefficients): coefficients(coefficients) {}

    /**
     * @brief A polynomial modulo X^N + 1 with coefficients in Z_{2^32} (this approximates
     * R/Z from TFHE).
     */
    Polynomial() = default;
};
