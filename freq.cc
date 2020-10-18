//
// freq.cc
//
// This implements the operations for the unordered dictionary data
// structure with the type `freq::dict*` for Project 1 of the Spring
// 2020 offering of CSCI 221.  Specifically, we define the functions
// for a bucket hash table that stores a collection of words and their
// integer counts.
//
// It is described more within the README for this project.
//
// The functions it defines include
//    * `int hashValue(std::string,int)`: give the hash location for a key
//    * `freq::dict* freq::build(int,int)`: build a word count dictionary 
//    * `int freq::totalCount(freq::dict*)`: get the total word count
//    * `int freq::numKeys(freq::dict*)`: get number of words
//    * `void freq::increment(freq::dict*,std::string)`: bump a word's count 
//    * `int freq::getCount(freq::dict*,std::string)`: get the count for a word
//    * `freq::entry* freq::dumpAndDestroy(freq::dict*)`: get the word counts, sorted by frequency
//    * `void freq::rehash(freq::dict*)`: expand the hash table
//
// The top four are implemented already, the other four need to be written.
//

#include <string>
#include <iostream>
#include "freq.hh"


// * * * * * * * * * * * * * * * * * * * * * * *
//
// HELPER FUNCTIONS FOR CHOOSING HASH TABLE SIZE
//

// isPrime(n)
//
// Return whether or not the given integer `n` is prime.
//
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

// primeAtLeast(n)
//
// Return the smallest prime number no smaller
// than `n`.
//
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

// * * * * * * * * * * * * * * * * * * * * * * *
//
// HELPER FUNCTIONS FOR COMPUTING THE HASH VALUE
//

// charToInt(c):
//
// Returns an integer between 0 and 31 for the given character. Pays
// attention only to letters, the contraction quote, "stopper" marks,
// and space.
//
//
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

// hashValue(key,tableWidth):
//
// Returns an integer from 0 to tableWidth-1 for the given string
// `key`. This serves as a hash function for a table of size
// `tableWidth`.  
//
// This method treats the string as a base-32 encoding of the integer
// it computes, modulo `tableWidth`. It relies on `charToInt` defined
// just above.
//
int hashValue(std::string key, int modulus) {
  int hashValue = 0;
  for (char c: key) {
    // Horner's method for computing the value.
    hashValue = (32*hashValue + charToInt(c)) % modulus;
  }
  return hashValue;
}

// insertAt(a,value,index,size)
//
// inserts `value` at `index` in the array `a`, shifting all values
// to the right of `index` to the right and deleting the last value
// of the array.
//
// used to dump a dictionary properly
//
void insertAt(freq::entry* a, freq::entry* value, int index, int size) {
  for (int i = size-1; i > index; i--) { 
    if (i > 0) { // just in case; don't want to go too far back
      a[i] = a[i-1]; // shift everything up to index to the right
    }
  }
  a[index] = *value;
}

// * * * * * * * * * * * * * * * * * * * * * * *
//
// Operations on freq::dict, and other support functions.
//
namespace freq {

//    * `int hashValue(std::string,int)`: give the hash location for a key
//    * `freq::dict* freq::build(int,int)`: build a word count dictionary 
//    * `int freq::totalCount(freq::dict*)`: get the total word count
//    * `int freq::numKeys(freq::dict*)`: get number of words
//    * `void freq::increment(freq::dict*,std::string)`: bump a word's count 
//    * `int freq::getCount(freq::dict*,std::string)`: get the count for a word
//    * `freq::entry* freq::dumpAndDestroy(freq::dict*)`: get the word counts, sorted by frequency
//    * `void freq::rehash(freq::dict*)`: expand the hash table
  // buildBuckets(howMany):
  //
  // Return an array of buckets of length `howMany`.
  //
  bucket* buildBuckets(int howMany) {
    bucket* bs = new bucket[howMany];
    for (int i=0; i<howMany; i++) {
      bs[i].first = nullptr;
    }
    return bs;
  }

  // build(initialSize,loadFactor):
  //
  // Build a word count dictionary that is roughly the given size, and
  // maintains the given load factor in its hash table.
  //
  dict* build(int initialSize, int loadFactor) {
    dict* newD = new dict;
    newD->numIncrements = 0;
    newD->numEntries    = 0;
    newD->loadFactor    = loadFactor; 
    newD->numBuckets    = primeAtLeast(initialSize);
    newD->buckets       = buildBuckets(newD->numBuckets);
    return newD;
  }

  // numKeys(D):
  //
  // Gives back the number of entries stored in the dictionary `D`.
  //
  int numKeys(dict* D) {
    return D->numEntries;
  }

  // totalCount(D):
  //
  // Gives back the total of the counts of all the entries in `D`.
  //
  int totalCount(dict* D) {
    return D->numIncrements;
  }


  // getCount(D,w):
  //
  // Gets the count associated with the word `w` in `D`.
  //
  int getCount(dict* D, std::string w) {
    bucket bucket = D->buckets[hashValue(w,D->numBuckets)];
    entry* current = bucket.first;
    while ( current != nullptr && current->word != w ) {
      current = current->next;
    }
    return (current == nullptr ? 0 : current->count); // I hope it's ok to use ternary operator
  }

  // rehash(D):
  //
  // Roughly doubles the hash table of `D` and places its entries into
  // that new structure.
  //
  void rehash(dict* D) {
    // UNIMPLEMENTED
    return;
  }

  // increment(D,w):
  //
  // Adds one to the count associated with word `w` in `D`, possibly
  // creating a new entry.
  //
  void increment(dict* D, std::string w) {
    // UNIMPLEMENTED
    int hash = hashValue(w,D->numBuckets);
    bucket bucket = D->buckets[hash]; // Creates a copy of the bucket, but the entries are pointers so its ok
    if (getCount(D, w) == 0) {
      if (bucket.first == nullptr) {
        bucket.first = new entry;
        bucket.first->word = w;
        bucket.first->count = 1;
        bucket.first->next = nullptr;
      } else {
        entry* current = bucket.first;
        while (current->next != nullptr) {
          current = current->next;
        }
        current->next = new entry;
        current->next->word = w;
        current->next->count = 1;
        current->next->next = nullptr;
      }
    } else {
      entry* current = bucket.first;
      while (current->word != w) {
        current = current->next;
      }
      current->count++;
    }
    return;
  }

  // dumpAndDestroy(D):
  //
  // Return an array of all the entries stored in `D`, sorted from
  // most to least frequent.
  //
  // Deletes all the heap-allocated components of `D`.
  //
  entry* dumpAndDestroy(dict* D) {
    // UNIMPLEMENTED
    entry* es = new entry[D->numEntries];
    int entryIndex = 0;
    for (int bucketIndex = 0; bucketIndex < D->numBuckets; bucketIndex++) { // iterate through buckets
      entry* current = D->buckets[bucketIndex].first;
      while (current != nullptr) { // iterate through entries in the bucket
        int i = 0;
        for (int ind = 0; es[ind].count > current->count && entryIndex > ind; ind++, i = ind); // iterate through
        // the array until you get to undefined parts or a place where the current index is <= current->count
        entryIndex++;
        insertAt(es,current,i,D->numEntries); // calls insertAt to place current at index
        current = current->next;
      } // this way is pretty complex, there might be a faster solution
    } // es should be built, now just to delete D
    //(*sizep) = entryIndex + 1;
    
    for (int bucketIndex = 0; bucketIndex < D->numBuckets; bucketIndex++) { // iterate through buckets
      entry* current = D->buckets[bucketIndex].first;
      while (current != nullptr) {
        entry* last = current;
        current = last->next;
        delete last;
      }
      //delete &(D->buckets[bucketIndex]);
    }
    delete D->buckets;
    delete D;

    return es;
  }

} // end namespace freq

