#ifndef _GRAM_H
#define _GRAM_H

namespace gram {

  // List of following words.
  struct follower {
    std::string word;
    struct follower* next;
  };

  // Word/bigram dictionary entry.
  struct gram {
    std::string words;    // Either a word or a pair of words separated by a space.
    int number;           // The number of followers of that word/bigram.
    follower* followers;  // The list of words that follow that word/bigram.
    struct gram* next;    // Another entry in this dictionary.
  };

  struct dict {
    gram* first;
  };

  dict* build(void);
  void add(dict* d, std::string ws, std::string fw);
  void add(dict* d, std::string w1, std::string w2, std::string fw);
  std::string get(dict* d, std::string k1, std::string k2);
  std::string get(dict* d, std::string k);
}

#endif // _GRAM_H
