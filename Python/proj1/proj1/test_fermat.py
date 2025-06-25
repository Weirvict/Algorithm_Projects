# Tests/test_fermat_functions.py
import random
from fermat import checkEvenOrOdd, mod_exp, fprobability, fermat, mprobability, miller_rabin

def test_checkEvenOrOdd():
    # Test cases for checkEvenOrOdd function
    assert checkEvenOrOdd(4) == "Even"
    assert checkEvenOrOdd(7) == "Odd"

def test_mod_exp():
    # Test cases for mod_exp function
    assert mod_exp(2, 3, 5) == 3
    assert mod_exp(3, 4, 7) == 4

def test_fprobability():
    # Test cases for fprobability function
    assert fprobability(3) == 87.5
    assert fprobability(5) == 96.875

def test_mprobability():
    # Test cases for mprobability function
    assert mprobability(2) == 75.0
    assert mprobability(4) == 93.75

def test_fermat():
    # Test cases for fermat function
    assert fermat(2, 1) == "prime"
    assert fermat(4, 1) == "composite"
    assert fermat(17, 5) == "prime"
    assert fermat(18, 5) == "composite"

def test_miller_rabin():
    # Test cases for miller_rabin function
    assert miller_rabin(2, 1) == "prime"
    assert miller_rabin(4, 1) == "composite"
    assert miller_rabin(17, 5) == "prime"
    assert miller_rabin(18, 5) == "composite"

if __name__ == "__main__":
    # Run all test functions
    test_checkEvenOrOdd()
    test_mod_exp()
    test_fprobability()
    test_mprobability()
    test_fermat()
    test_miller_rabin()
    print("All tests passed!")
