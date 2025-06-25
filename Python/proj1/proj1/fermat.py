import random
from math import floor

"""
Victoria Weir
CS4412
Project 1 - Primality Test

"""


def prime_test(N, k):
    # This is main function, that is connected to the Test button. You don't need to touch it.
    return fermat(N, k), miller_rabin(N, k)


"""
Odd/Even Checker
:param number: The number to be checked.
:return: 'Even' if the number is even, 'Odd' otherwise.

Check if a given number is even or odd.
"""


def checkEvenOrOdd(number):
    if (number % 2) == 0:  # O(1)
        return "Even"
    else:  # O(1)
        return "Odd"


"""
Compute modular exponentiation.
:param x: Base
:param y: Exponent
:param N: Modulus
:return: Result of x^y mod N

This function uses a recursive algorithm for efficient modular exponentiation.
"""


def mod_exp(x, y, N):
    if y == 0:
        return 1  # O(1)
    else:  # O(n)
        # This starts recursion
        z = mod_exp(x, floor(y / 2), N)  # O(log(n)
        if checkEvenOrOdd(y) == "Even":  # (O(1)
            return pow(z, 2, N)
        else:  # O(1)
            return x * pow(z, 2, N)
    return 1


"""
Probability of Fermat
:param k: Number of Fermat trials
:return: Probability as a percentage

Calculate the probability of Fermat's primality test giving the correct answer.
"""


def fprobability(k):  # O(1)
    # Based off of Pr(Algorithm 1.8 returns yes when N is not prime) <= 1/2^k
    numb = 1 / (pow(2, k))
    return (1 - numb) * 100  # convert to percentage


"""
Probability of Miller-Rabin
:param k: Number of Miller-Rabin trials
:return: Probability as a percentage
    
Calculate the probability of Miller-Rabin primality test giving the correct answer.
"""


def mprobability(k):  # O(1)
    # You will need to implement this function and change the return value.
    # Based off of fprobablity but denominater is doubled
    numb = 1 / (pow(4, k))
    return (1 - numb) * 100  # convert to percentage


"""
Fermat Primality Test
:param N: Positive integer to be tested for primality.
:param k: Number of random tests to perform.
:return: 'prime' if likely prime, 'composite' otherwise.

Perform Fermat primality test to determine if a positive integer N is likely prime.
"""


def fermat(N, k):
    # Check if N is 2 (a prime number)
    if N == 2:  # O(1)
        return 'prime'
    # Check if N is even (a composite number)
    if N % 2 == 0:  # O(1)
        return 'composite'

    # Start of Fermat primality test algorithm
    for i in range(k):  # O(k)
        # Generate a random integer a in the range (1, N-1)
        a = random.randint(1, N - 1)  # O(1)
        # Check if a^(N-1) mod N is not congruent to 1 (composite)
        if mod_exp(a, N - 1, N) % N != 1:  # O(logn)
            return 'composite'
        # k * logn
    # If all tests pass, N is likely prime
    return 'prime'  # O(1)


"""
Miller-Rabin Primality Test
:param N: Positive integer to be tested for primality.
:param k: Number of random tests to perform.
:return: 'prime' if likely prime, 'composite' otherwise.

Perform Miller-Rabin primality test to determine if a positive integer N is likely prime.
"""


def miller_rabin(N, k):
    # Check if N is less than or equal to 3 (a prime number)
    if N <= 3:  # O(1)
        return 'prime'

    # Check if N is even (a composite number)
    if checkEvenOrOdd(N) == 'Even':  # O(1)
        return 'composite'

    # Start of Miller-Rabin primality test algorithm
    for i in range(k):  # O(k)
        # Generate a random integer a in the range (1, N-1)
        a = random.randint(1, N - 1)  # O(1)

        # Compute a^(N-1) mod N
        computeMod = mod_exp(a, N - 1, N)  # O(n^2)

        # Check conditions for probable primality
        if computeMod == 1 or computeMod == N - 1:  # O(n)
            return 'prime'
        else:
            computeEvenFurther = (computeMod * computeMod) % N  # O(logn)
            if computeEvenFurther == 1:  # O(1)
                return 'composite'
            if computeEvenFurther == N - 1:  # O(1)
                return 'prime'

        # k * (n^2*logn) = k*log^2n

    # If all tests pass, N is likely composite
    return 'composite'


if __name__ == "__main__":
    # Test the functions with some sample values
    N_values = [4, 8, 17, 25, 32, 47, 58, 63, 78, 85, 92, 100]
    k_value = 5

    for N in N_values:
        fermat_result, miller_rabin_result = prime_test(N, k_value)
        fermat_probability = fprobability(k_value)
        miller_rabin_probability = mprobability(k_value)

        print(f"Fermat test result for {N}: {fermat_result}")
        print(f"Probability for Fermat test: {fermat_probability}%")
        print(f"Miller-Rabin test result for {N}: {miller_rabin_result}")
        print(f"Probability for Miller-Rabin test: {miller_rabin_probability}%")
        print("-" * 30)