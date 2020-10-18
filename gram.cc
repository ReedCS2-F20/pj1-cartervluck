#include <string>
#include <iostream>
#include "gram.hh"
#include <ctime>
#include <cstdlib>

bool isPrime(int n) {
  // Handle the obvious cases, including even ones.
  if ((n <= 2) || (n % 2 == 0)) {
    return (n == 2);
  }
  // Try several odd divisors.
  int d = 3;
  while (d*d <= n) {
    if (n % d == 0) {
      // It has a divisor. It's not prime.
      return false;
    }
    d += 2;
  }
  // No divisors. It's prime.
  return true;
}

int primeAtLeast(int n) {
  if (n <= 2) {
    return 2;
  }
  int p = 3;
  while (p < n || !isPrime(p)) {
    p += 2;
  }
  return p;
}

int charToInt(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 1;
  } else if (c == '.') {
    return 27;
  } else if (c == '!') {
    return 28;
  } else if (c == '?') {
    return 29;
  } else if (c == '\'') {
    return 30;
  } else if (c == ' ') {
    return 31;
  } else {
    return 0;
  }
}

int hashValue(std::string key, int modulus) {
  int hashValue = 0;
  for (char c: key) {
    // Horner's method for computing the value.
    hashValue = (32*hashValue + charToInt(c)) % modulus;
  }
  return hashValue;
}

namespace gram {

  dict* build(void) {
    srand(time(0));
    dict* newD = new dict;
    return newD;
  }

  std::string get(dict* D, std::string ws) {
    return "hello"; // UNIMPLEMENTED
  }

  std::string get(dict* D, std::string w1, std::string w2) {
    return get(D,w1+" "+w2);
  }
             
  void add(dict* D, std::string ws, std::string fw) {
    return; // UNIMPLEMENTED
  }
  
  void add(dict* D, std::string w1, std::string w2, std::string fw) {
     add(D,w1+" "+w2,fw);
  }
     
  void destroy(dict *D) {
    delete D;
  }
} 

