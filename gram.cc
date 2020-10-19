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
    newD->first = nullptr;
    return newD;
  }

  std::string get(dict* D, std::string ws) {
    gram* currentGram = D->first;
    while (currentGram != nullptr && currentGram->words != ws) {
      currentGram = currentGram->next;
    }
    if (currentGram == nullptr) {
      return ""; // Invalid lookup. should never happen, as every word in text should have a value by the time we do this kind of lookup
    }
    if (currentGram->followers == nullptr) {
      return ""; // End behavior; that is, eof. hopefully there's some catch for it in chats.cc?
    }
    int randIndex = rand() % currentGram->number;
    follower* currentFollower = currentGram->followers;
    for (int i = 0; i < randIndex; i++) { 
      currentFollower = currentFollower->next;
    }
    return currentFollower->word;
  }

  std::string get(dict* D, std::string w1, std::string w2) {
    return get(D,w1+" "+w2);
  }
             
  void add(dict* D, std::string ws, std::string fw) {
    gram* currentGram = D->first;
    while (currentGram != nullptr && currentGram->words != ws) {
      currentGram = currentGram->next;
    }
    if (currentGram == nullptr) {
      gram* newGram = new gram; // create new gram to push to front of dict
      newGram->number = 1;
      newGram->followers = new follower;
      newGram->followers->word = fw;
      newGram->followers->next = nullptr;
      newGram->words = ws;
      newGram->next = D->first;
      D->first = newGram;
    } else {
      currentGram->number++; // increment number of followers for the word
      follower* newFollower = new follower;
      newFollower->word = fw;
      newFollower->next = currentGram->followers; // push new word to front of linked list
      currentGram->followers = newFollower;
    }
    return;
  }
  
  void add(dict* D, std::string w1, std::string w2, std::string fw) {
     add(D,w1+" "+w2,fw);
  }
     
  void destroy(dict *D) {
    delete D;
  }
} 

