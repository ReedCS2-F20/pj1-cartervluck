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
    std::cout << "LOG: REHASHING!!!" << std::endl;
    int nbuckets = primeAtLeast(D->numBuckets*2);
    bucket* newArray = buildBuckets(nbuckets); // heap allocated array of buckets
    for (int i = 0; i < D->numBuckets; i++) { // iterate through buckets
      entry* current = D->buckets[i].first;
      while (current != nullptr) { // iterate through every item in the bucket
        entry* front = newArray[hashValue(current->word,nbuckets)].first; // pointer to first entry in this bucket
        entry* newEntry = new entry; // create a new entry in the front of the array
        newEntry->next = front;
        newEntry->word = current->word;
        std::cout << "LOG: just copied " << newEntry->word << std::endl;
        newEntry->count = current->count;
        newArray[hashValue(current->word,nbuckets)].first = newEntry;
        entry* last = current;
        current = current->next;
        delete last; // because we're creating a new entry we need to dealloc the old current
      }
    } // newArray has been constructed
    std::cout << "LOG: just rehashed, feeling good" << std::endl << "LOG: new bucket count: " << nbuckets << std::endl;
    bucket* oldArray = D->buckets; // points to the start of the old array
    D->buckets = newArray; // points to the start of the new array
    D->numBuckets = nbuckets;
    delete oldArray;
    return;
  }

  // increment(D,w):
  //
  // Adds one to the count associated with word `w` in `D`, possibly
  // creating a new entry.
  //
  void increment(dict* D, std::string w) {
    std::cout << "LOG: incrementing." << std::endl;
    int hash = hashValue(w,D->numBuckets);
    bucket bucket = D->buckets[hash]; // Creates a copy of the bucket, but the entries are pointers so its ok
    if (getCount(D, w) == 0) { // the word is not in the dict yet
      std::cout << "LOG: adding new word: " << w << std::endl;
      std::cout << "LOG: now we have " << D->numEntries + 1 << " words. Num Buckets: " << D->numBuckets << "." << std::endl;
      if (D->numEntries/D->numBuckets >= D->loadFactor) {
        rehash(D); // rehash D if adding a new entry will exceed the load factor
      }
      if (bucket.first == nullptr) { // if the bucket is empty, create a new entry at the start
        bucket.first = new entry;
        bucket.first->word = w;
        bucket.first->count = 1;
        bucket.first->next = nullptr;
        D->buckets[hash] = bucket; // because this is a copy of bucket, we need to update D's version
      } else {
        entry* current = bucket.first; // we are referencing the same data as D, so we're good just changing current
        while (current->next != nullptr) {
          current = current->next;
        }
        current->next = new entry;
        current->next->word = w;
        current->next->count = 1;
        current->next->next = nullptr;
      }
      D->numEntries++;
    } else { // the word is in the dict
      std::cout << "LOG: Add 1 instance of " << w << std::endl;
      entry* current = bucket.first; // current points to the heap, as does bucket.first so we can change current
      while (current->word != w) {
        current = current->next;
      }
      current->count++;
    }
    D->numIncrements++;
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
        //std::cout << "Deleting " << current->word << std::endl;
        delete last;
      }
      //delete &(D->buckets[bucketIndex]);
    }
    delete D->buckets;
    delete D;

    return es;
  }

} // end namespace freq

