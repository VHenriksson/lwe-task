#pragma once
#include <vector>
#include "polynomial.h"

template <uint16_t k, uint16_t N>
struct CipherText
{
    std::array<Polynomial<N>,k> a;
    Polynomial<N> b;

    friend bool operator==(const CipherText& lhs, const CipherText& rhs)
    {
        if (lhs.b != rhs.b)
        {
            return false;
        }
        for (uint16_t i = 0; i < lhs.a.size(); i++)
        {
            if (lhs.a[i] != rhs.a[i])
            {
                return false;
            }
        } 
        return true;
    }
    friend bool operator!=(const CipherText& lhs, const CipherText& rhs){return !(lhs == rhs);};    
    friend CipherText operator+(const CipherText& lhs, const CipherText& rhs){
        CipherText<k,N> result = {std::array<Polynomial<N>,k>(), Polynomial<N>(std::vector<uint32_t>{})};
        for (size_t i = 0; i < lhs.a.size(); i++)
        {
            result.a[i] = lhs.a[i] + rhs.a[i];
        }
        result.b = lhs.b + rhs.b;
        return result;
    }
    friend CipherText operator+=(CipherText& lhs, const CipherText& rhs){
        for (size_t i = 0; i < lhs.a.size(); i++)
        {
            lhs.a[i] += rhs.a[i];
        }
        lhs.b += rhs.b;
        return lhs;
    }
};