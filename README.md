Learning With Errors (or by making them)
========================================

The following repository contains a simple (and most likely not secure) implementation of a homomorphic encryption scheme based on the Learning With Errors (LWE) problem.

**Note**: This README relies heavily on $\LaTeX$-style math notation. It seems to render fine on GitHub and in the VS Code markdown preview, but I have not tested it elsewhere.

Usage
-----

**Building**:

The code uses cmake. For testing, we use Catch2, but is besides that self contained.

To compile the code, you can for instance run:

```bash
cd build
cmake ..
cmake --build .
```
or
```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
for an optimized build.

**Interface**:

There is no io interface for the code. The code is meant to be used as a library, and the `main` function is just for testing and profiling purposes.

The main star of this repository is the `SecretKey` template class. It takes two security parameters, $N$ the size of the polynomial, and $k$ the number of samples. The third parameter, the modulus $q$, is fixed to $2^{32}$ as seems common in TFHE. The user is responsible for their own security parameters, but we will do our best to provide some guidance in the section [Security Parameters](#security-parameters) below.

To start encrypting and decrypting, all that is needed is to initialize a `SecretKey` object as follows:

```cpp
SecretKey<k, N> sk;
```

The message space is the set $\underbrace{\mathbb{Z}_{16} \times \dots \times \mathbb{Z}_{16}}_{N \text{ times}}$. In other words, we put one four bit message on each coefficient of the polynomial. Messages should be encoded in the four *least significant bits* when given to the encryption function.

```cpp
std::array<int, N> m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
CipherText c = sk.encrypt(m);
```

`CipherText` is an object corresponding to an GLWE ciphertext in the flavour of TFHE. Two ciphertexts can be added, but no other operations has been implemented so far.

```cpp
CipherText c_1 = sk.encrypt(m_1);
CipherText c_2 = sk.encrypt(m_2);
CipherText c_3 = c_1 + c_2;
```
To decrypt, we use the `decrypt` method of the `SecretKey` object.

```cpp
std::array<int, N> m_dec = sk.decrypt(c);
```


Implementation
--------------

The implementation consists of three object templates:

**Polynomial**:

A class representing a polynomial with integer coefficients. It supports some basic operations like addition, subtraction, multiplication and equality. It uses a naive multiplciation algorithm with an O(n^2) complexity. It would be preferable to use (hardware accellerated :D ) FFT multiplication, but I could not make (even a regular one) work on the time I had.

**SecretKey**:

A class which sets up a secret key, and then may use it for encryption and decryption. The cryptosystem is supposed to be inspired by TFHE. The class handle the full key generation, as well as encryption and decryption.

The secret key is a `Polynomial` of degree $N-1$ with coefficients in $\{0,1\}$. 

For key and ciphertext generation, we use `std::random_device` for random number generation. This is not a cryptographically secure way to generate random numbers, since it leaves the responsibility to the underlying system. However, it is certainly good enough for this toy-implementaion.

**CipherText**:

Ciphertexts are objects on the form $(A, b)$, where $A$ is the public key, and $b$ the encrypted message. Here, $A$ is a list of $k$ `Polynomial` objects, each of degree $N-1$ with coefficients in $\mathbb{Z}_{2^{32}}$, and $b$ is one such polynomial.

 a `Polynomial` of degree $N-1$ with coefficients in $\{0,1\}$, and $b$ is a `Polynomial` of degree $N-1$ with coefficients in $\{0,1\}$. The ciphertexts are encrypted using the secret key, and can be added together.

Performance considerations
--------------------------

One clear performance issue with the current implementation is the lack of FFT multiplication. 

Because I did not manage to get FFT to work, it gave me a bit less abilities to optimize the rest of the code. Since the c++-compiler is really clever, I often find it difficult to know how changes will affect the performance. To me, the only real way to progress is to profile the code.

I did a small attempt at profiling, and produced the following flamegraph:

![Flamegraph](flamegraph_tfhe.svg)

The flamegraph shows that the majority of the time is spent in the multiplication operator. The reasonable interpretation of this is that it is indeed the absense of FFT multiplication that is the bottleneck. One could probably dig deeper into this, and learn more about the performance even in the absence of FFT multiplication, but that would require more time and effort.

One example of why I find it so important to measure is when I tried to make an optimization which I thought would be an obvious win. I have checked in an published the optimization in the branch `direct_access_to_polynomial`. Looking at the `encrypt` method in the `SecretKey` class, it currently copies the array `plain` in the function definition. Then it copies it again to create the polynomial which is added to `cipher.b`.

I thought that it should be possible to get rid of these two to copies, and instead add the shifted plain elements directly to the polynomial coefficients. However, in my attempt, this turned out to be slower than the current implementation.

This might of course be because I made some error in my implementation. But it might also be that the compiler is able to optimize the code in a way that I do not fully understand. Maybe the explanation has something to do with memory locality, where the program find it easier to place the copy of `plain` close to the "old" `plain` in order to do the shifting and adding of error, and then move all of it closer to the polynomial in order to do the addition. But this is very much just speculation.


Security Parameters
-------------------