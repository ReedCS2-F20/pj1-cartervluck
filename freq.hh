#ifndef _FREQ_H
#define _FREQ_H

// freq.hh
//
// This defines the data structure for Project 1 of the Spring 2020
// offering of CSCI 221, along with its public interface of functions.
// 
// This defines a bucket hash table as a type `freq::dict*` that stores
// a collection of words and their integer counts.
//
// It is described more within the README.
//

namespace freq {

  // entry
  //
  // A linked list node for word/count entries in the dictionary.
  //
  struct entry {

    std::string word;  // The word that serves as the key for this entry.

    int count;         // The integer count associated with that word.

    struct entry* next;
  };

  // bucket
  //
  // A bucket serving as the collection of entries that map to a
  // certain location within a bucket hash table.
  //
  struct bucket {

    entry* first;  // It's just a pointer to the first entry in the
		   // bucket list.
  };

  // dict
  //
  // The unordered dictionary of word/count entries, organized as a
  // bucket hash table.
  //
  struct dict {

    bucket* buckets;   // An array of buckets, indexed by the hash function.

    int numIncrements; // Total count over all entries. Number of `increment` calls.

    int numBuckets;    // The array is indexed from 0 to numBuckets.

    int numEntries;    // The total number of entries in the whole
		       // dictionary, distributed amongst its buckets.

    int loadFactor;    // The threshold maximum average size of the
		       // buckets. When numEntries/numBuckets exceeds
		       // this loadFactor, the table gets rehashed.
  };

  //
  // The public interface to freq::dict objects.
  //
  dict* build(int initialSize, int loadFactor); // Constructs and returns a new `freq::dict`.

  int numKeys(dict* D);                         // Returns the number of entries in `D`. 
  int totalCount(dict* D);                      // Returns the number of `increment` calls made on `D`.

  void increment(dict* D, std::string k);       // Updates the count of a word `k` in `D`.

  int getCount(dict* D, std::string k);         // Gets the count of word `k` in `D`.

  entry* dumpAndDestroy(dict* D);               // Gives back a summary of `D` and returns its storage of
                                                // back to the heap. Communicates the number of entries
                                                // in the summary using `sizep`.

}

#endif // _FREQ_H
