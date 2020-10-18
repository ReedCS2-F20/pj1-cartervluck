//
// stats.cc
//
// Count and report statistics about the number of occurrences of
// words within some entered text.
//
// Compile with: g++ -g --std=c++11 -o stats stats.cc freq.cc
//
// Usage: ./stats
//
// The program will process a series of lines of text, looking for
// contiguous runs of alphabetic characters treating them each as a
// word. For each such word, it keeps a count of the number of its
// occurrences in the text.
//
// To end text entry hit RETURN and then CTRL-d.  It then reports a
// summary of the words it found, along with their counts.
//
// Alternative usage: ./stats < textfile.txt
//
// The above will instead process the text of the file in
// 'textfile.txt'.
//

//
// This implementation relies on a word count dictionary implemented
// as a bucket hash table as defined in "freq.hh".
//

#include <iostream>
#include "freq.hh"



// next_word_in(line):
//
// Process the characters of the std::string object 'line', seeking
// the next contiguous sequence of letters within that string.
//
// Once it has processed that next word, it modifies 'line' to exclude
// the prefix of processed characters from it, and returns that next
// word as a string. The word will be a string of lowercase alphabetic
// characters.
//
// It returns the empty string once the line has been fully processed.
//
std::string next_word_in(std::string &line) {
  std::string word = "";
  int i;
  for (i=0; i < line.size(); i++) {
    char c = line[i];

    // Make letters lowercase.
    if ('A' <= c && c <= 'Z') {
      c = c+32;
    }

    // Include letters or contraction marks as part of a word.
    if (('a' <= c && c <= 'z') || c == '\'') {
      word += c;

    // If we hit a "stopper", emit it as a word.   
    } else if (c == '.' || c == '!' || c == '?') {
      if (word.size() > 0) {
	line.erase(0,i);
	return word;
      } else {
	word += c;
	line.erase(0,1);
	return word;
      }

    // If we hit any non-letter, emit a word.
    } else {
      if (word.size() > 0) {
	line.erase(0,i);
	return word;
      }
    }
  }
  // End of the line, emit that ending word.
  line.erase(0,i);
  return word;
}
  
// main()
//
// Processes STDIN as a sequence of words. Using a htable::htable, tracks
// the number of occurrences of words in that entered text.
//
// It reports the word counts to STDOUT.
//
int main(int argc, char **argv) {

  //
  // Build a dictionary of word:count entries based on the text entered.
  freq::dict *d = freq::build(9,2);
  std::cout << "READING text from STDIN. Hit ctrl-d when done entering text.\n";

  // Read until the end of text entry.
  while (std::cin) {

    // Get the next line of entered text.
    std::string line;
    std::getline(std::cin,line);

    // Read each of the words in that line of text.
    for (std::string w = next_word_in(line); w != ""; w = next_word_in(line)) {
      freq::increment(d,w);
    }
  }
  std::cout << "DONE.\n";
  std::cout << "HERE are the word statistics of that text:\n";

  //
  // Report some basic statistics.
  int wordCount = freq::totalCount(d);
  int numWords  = freq::numKeys(d);
  std::cout << std::endl;
  std::cout << "That text was " << wordCount << " words in length." << std::endl;
  std::cout << std::endl;
  std::cout << "There are " << numWords << " distinct words used in that text." << std::endl;

  //
  // Get the full word count analysis.
  freq::entry* words = freq::dumpAndDestroy(d);
  int top = 1;
  if (numWords > 10) {
    top = 10;
    if (numWords > 100) {
      top = 100;
    }
  }
  std::cout << std::endl;
  std::cout << "The top " << top << " ranked words (with their frequencies) are:" << std::endl;
  for (int i=0; i<top-1; i++) {
    std::cout << (i+1) << ". " << words[i].word << ":" << words[i].count << ", ";
  }
  std::cout << top << ". " << words[top-1].word << ":" << words[top-1].count << std::endl;

  int count = 0;
  while (count < numWords && words[count].count > 1) {
    count++; 
  }
  std::cout << std::endl;
  std::cout << "Among its "<< numWords << " words, " << (numWords-count) << " of them appear exactly once." << std::endl;
}
    

