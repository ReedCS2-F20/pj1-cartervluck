# Project 1: text stats and chats

*Due Friday, October 30th, by 11:59pm*

This is a two-part project that is a capstone of the C++ programming
we learned in the first 6 weeks. It uses many of the components we
built separately in the lab and in homework. It consists of two
parts. The first part as a text analyzer that reports statistics about
word use in the text it processes. The second part builds from the
same idea, but instead analyzes the *bi-grams* and *tri-grams* (runs
of 2 or 3 words) of the text it processes, training a *Markov
process*, so as to use that process to generate a random text that
mimics the writing of the text by which it was trained.

The first part will feel quite a bit like a homework exercise, but that
is by design: my goal is to have you get Part 1 working quickly, and then
also to learn the ideas necessary for completing Part 2. The second part
is a bit more open-ended: you are asked to develop your own data structure
that represents the random process that generates the random text.

### Text analysis

There is a growing area of the study called *digital humanities*
that treats human-written texts as *data*, and so subject to the same
kinds of analysis that scientists and social scientists my use to
reason about their objects of study. By treating a text this way,
for example, a humanist may get insights into an author's use of
language. What is the structure of their prose? What word choices
to they tend to make? Do they have a signature style that can be
quantified in some way? In some case these analyzed aspects might help
us identify an author of an anonymous text, or be used as evidence to
support or reject hypothesized authors.

The first cut at such analyses look at word frequencies. Suppose, in a
1000 page text, the author used 20000 words only once? Or they used
only 10000 different words total? Does that say anything about their
style? Maybe they use verbs in a passive versus and active tense.  Do
their sentences tend to be long or short?  Etc. Alternatively: how
many times does a character's name appear in a certain novel?

The algorithms and data structures that support these kinds of
analyses, it turns out, are fairly foundational in general
computing. The same techniques you might use to analyze a text, can
also be used to study genetic sequences, or protein structure, and
maybe even general data that makes up our files' and programs'
sequences of bits.

Related to this is the question of information content, the essential
make-up of text, digitally speaking. Can the document be compressed so
that it uses fewer bits? Can we summarize some of its content? Can we
make its ideas searchable? Can we relate it to other documents in some
way?

### Text synthesis

Also related to these notions of how we characterize a text digitally
is a question of *text generation*. Can we use our techniques to
generate novel, and maybe even meaningful, sequences of words? As we
hear in the news, there are *bots* that post information on sites,
send spam emails, and in some near future may even interact with us in
text or in speech in novel ways.

We are in the midst of a technical revolution in this regard: deep
learning and statistical techniques are able to represent parsable
sentence structure in some remarkable ways and can string words
together
[so as to have seeming meaning and content.](https://www.newyorker.com/magazine/2019/10/14/can-a-machine-learn-to-write-for-the-new-yorker)
Nevertheless,
there is much work to be done, and every development highlights the
difficulty of natural language processing and synthesis, the huge
complexity that must clearly be involved in human thought and
communication.

For this project we build a very simple model for synthesizing sequences of
words that mimiic human-written texts, albeit in a very limited way.
We build a *stochastic process* by analyzing the frequency of the third
word of every three-word sequence in a text. The idea here is that,
when we are reading a text (say, the words you are reading *right now*)
our minds build expectations for the next word we read based on the last
few words we've just read. 

If you read "The dog...", say, you might not be surprised if the next
word is "barked", or "sat", or "slept," and maybe even "park" or
"house." You might not expect "purred," or "philosophized," or
"rented," and probably not "candelabra" or "eight."  And, maybe having
then read "The dog barked", you might be surprised by "sat" but not by
"loudly."  So, maybe processing the text of a novel whose main
character has a faithful dog companion, the *bi-gram* "the dog" might
occur 12 times, and have "slept" follow 3 of those times, "barked"
happen 5, "sat" happen twice, and "park" follow twice.  And maybe "dog
barked" would have "at" happen several times (even more than 6, if
"his dog barked" was also a common triple), but also "loudly" twice,
and so on.

We can turn this analysis around to define a generation
process. Suppose we decided to, while writing a novel, start with the
words "The dog", we could then chooose where to go next in our writing
totally at random. We toss a 12-sided die, choosing to type "slept" if
1, 2, or 3 comes up, "barked" if 4 through 8 comes up, "sat" if 9 or
10, and "park" if we roll an 11 or 12.  And say we rolled a 10.  We
type "sat" and then play the same random game to maybe choose between
"down" or "between" or "on", etc. Continuing this way, and we've got
ourself a novel.

---

### Part 1: text analysis with `stats`

**Overview**  
In this folder you'll find the starting code for this assignment, namely three C++ 
source files:

• `stats.cc`: a description of an unfinished `main` procedure that processes a sequence of 
input words, and puts them into an unordered dictionary of type `freq::dict*`, tracking
the number of times each word occurs in the text as a whole. It then, with your changes,
will output some statistics about the words in that text, devised from that dictionary.

• `freq.hh`: a header file defining the `freq::dict` struct and operations on it and
its subcomponents, used for counting the occurrences of words. The data structure, as we'll
describe below, is a *bucket hashtable*, one that resizes itself if its number of entries
gets too big.

• `freq.cc`: the implementation of the unordered dictionary mentioned just above.

Your assignment is to complete the implementation of the `freq::` functions and procedures,
and then modify/extend `main` so that it reports certain statistics described below.

Once you've written everything, your program should have the kind of interaction
below:

    $ ./stats
    READING text from STDIN. Hit ctrl-d when done entering text.
    Fair Reed
    All hail, Alma Mater, new born of the West
    Where the mighty Columbia flows,
    Thou fortunate heir of the pioneers' quest,
    All hail to the griffin and rose.
    Thou wilt cherish thine own through woe and through weal,
    Thou wilt throw on the pathway a light,
    Never dimming the truth thru a partisan's zeal,
    Never doubting the triumph of right.
    DONE.
    HERE are the word statistics of that text:

    There are 46 distinct words used in that text.

    The top 10 ranked words (with their frequencies) are:
    1. the:7, 2. thou:3, 3. of:3, 4. never:2, 5. a:2, 6. through
    :2, 7. wilt:2, 8. and:2, 9. hail:2, 10. all:2

    Among its 46 words, 35 of them appear exactly once.

You should know that, in the above, the lines from `Fair Reed` down to
the line above `DONE.` were entered by the user into the console.
After they hit the `[RETURN]` key, they they hit `[CONTROL-d]` to
signal the end of text (this is the console's "end of file" keystroke),
and then the program produced the subsequent output.

Below is another run, but using a special feature of the console called
*input redirection*. The `<` tells the console to feed in the contents
of the file `jabberwocky.txt` as if they are being typed in directly
by the user. Here is that run:

    $ ./stats < jabberwocky.txt
    READING text from STDIN. Hit ctrl-d when done entering text.
    DONE.
    HERE are the word statistics of that text:

    There are 94 distinct words used in that text.

    The top 10 ranked words (with their frequencies) are:
    1. the:19, 2. and:14, 3. !:11, 4. he:7, 5. in:6, 6. .:5, 7. 
    through:3, 8. my:3, 9. jabberwock:3, 10. went:2

    Among its 94 words, 57 of them appear exactly once.

This text file contains Lewis Carroll's somewhat gibberish-y poem
*Jabberwocky* and has lots of used-once words. There are several
used twice, and the code just happens to report one of these,
the word "went."

You'll notice that the code considers "stop punctuation", the marks
`.`, `!`, and `?` as if they are words. (This will be useful for text
synthesis, later.) It ignores other punctiation marks, and it treats
all letters as their lowercase. It currently fails to properly process
accented letters, ignoring them and only paying attention to `a`
through `z`, `A` through `Z`, and also the contraction mark `'`,
treating them as valid components that make up a word.

For shorter entries (fewer than 10 words), it only reports a most 
frequent word. For medium (fewer than 100), it only reports the
top 10. For longer texts, it reports the top 100. Here is an
example of it processing the full text of an English translation of
Gabriel García Márquez's *One Hundred Years of Solitude*:

    $ ./stats < hundred_years.txt 
    READING text from STDIN. Hit ctrl-d when done entering text.
    DONE.
    HERE are the word statistics of that text:

    There are 10384 distinct words used in that text.

    The top 100 ranked words (with their frequencies) are:
    1. the:10932, 2. .:5615, 3. of:5260, 4. and:4206, 5. to:3687
    , 6. that:3196, 7. a:3190, 8. in:3063, 9. he:2757, 10. was:2
    373, 11. had:2175, 12. her:2046, 13. with:2019, 14. she:1995
    , 15. his:1887, 16. on:1158, 17. it:1141, 18. him:1127, 19. 
    not:1090, 20. for:1052, 21. they:995, 22. as:877, 23. one:82
    8, 24. by:806, 25. would:792, 26. aureliano:783, 27. at:779, 
     28. but:746, 29. who:675, 30. when:671, 31. s:653, 32. were:
    652, 33. from:597, 34. so:564, 35. time:547, 36. ursula:512,
     37. an:504, 38. arcadio:480, 39. them:477, 40. did:471, 41. 
     up:444, 42. jose:439, 43. which:424, 44. house:418, 45. bee
    n:414, 46. out:405, 47. their:386, 48. only:372, 49. years:3
    71,50. be:370, 51. because:356, 52. buendia:349, 53. all:349
    , 54. into:342, 55. could:331, 56. about:322, 57. no:320, 58
    . have:317, 59. colonel:312, 60. amaranta:311, 61. more:301,
     62. said:298, 63. segundo:293, 64. then:265, 65. where:264,
     66. even:263, 67. like:255, 68. you:253, 69. solitude:250, 
    70. without:249, 71. hundred:248, 72. there:244, 73. put:235
    , 74. first:229, 75. went:228, 76. if:224, 77. fernanda:218,
    78. gabriel:217, 79. room:213, 80. two:210, 81. made:209, 82
    . over:208, 83. any:206, 84. garcia:206, 85. until:204, 86. 
    after:204, 87. i:202, 88. x:202, 89. marques:201, 90. took:2
    01, 91. before:194, 92. than:192, 93. through:191, 94. what:
    191, 95. other:187, 96. same:186, 97. man:185, 98. macondo:1
    77, 99. night:173, 100. back:173

    Among its 10384 words, 4266 of them appear exactly once.

**Bucket hashing**  

The unordered dictionary you'll devise will hold a collection of words
along with an integer count of the occurrences of each word. The
implementation you must provide is of a particular variety, called a
*hash map* or a *hash table*. Of those, it is a particular sort using
a method called *bucket hashing.* Hash tables, in general, are an
efficient means for storing a collection of items, and in no
particular order. The key thing that they provide is super fast search
of that data structure. They are implemented so as to provide instant
look up of a key in their dictionary, typically only a constant amount
of time gets spent searching for a key's entry, regardless of the
total number in that dictionary. 

The search is made fast in two ways. First, a major step of any search
involves (typically) a single access to an array location.  Since
array accesses take constant time (it's just a request from a certain
indexed spot in memory), this happens quickly. Second, there is a fast
scheme that's used to determine which position in the array a key's
entry lives. This is something called a *hash function*, something
I need to briefly describe.  

If you look at the definitions in `freq.cc` you'll see the definition 
of a function

    int hashValue(std::string key, int tableWidth) { ... }

This function takes two parameters, the string of the `key` we're
searching for, and also the size `tableWidth` of the array holding the
table.  These two pieces of information are used to quickly devise
and return an `int` location that is a value from `0` to `tableWidth-1`.
This gives a spot in the table's array where the entry for `key` should
live. 

We won't talk much here about how `hashValue` does that. It suffices to
say that the function always computes the same value for a given string
(it's deterministic). Also, it tends to, with any mix of data (i.e.
most *distributions* of strings) "spread* the keys around amongst the
`tableWidth` possible locations in the array. It does this, incidentally,
by relying on the `tableWidth` being a prime number.

That being said, there is one tricky aspect of hash tables,
particularly their hash functions: in general, you cannot assume that
each key "hashes" to a unique location, distinct from the other keys
stored in the table. Instead, we have to assume that two keys might
have the same `hashValue`.  This is called a *collision*, and
different hash table designs handle collisions in a variety of
ways. Collisions happen, and they can certainly happen in designs
where a hash table's array is smaller than the number of entries it
stores. In any case, when two or more keys map to the same spot in the
hash table's array, we need to resolve how to store all of them, and
still make it easy to find their entries.

We describe a *collision resolution* strategy called *bucket hashing*.
Here, each spot *i* in the hash table's array actually stores a linked
list. That serves as the collection (the "bucket") of entries that
all map to *i*. If two keys *x* and *y* both map to a location *i* in 
the array, then both their entries would be stored in that linked list
bucket.  To search for a key, to see if it has an entry, then, we
go directly to the correct bucket (according to `hashValue`) and
then we scan through the entries of its linked list.

**The code for `freq.cc`**

There are four major `freq::` functions used in `main` that allow
it to compute the word use statistics of a text. The first of
these is

• `build(initialSize,loadFactor)`: this code constructs an empty
bucket hash table whose array's initial size is a prime number
at least as large as the given `initialSize`.  It configures
it so that, as keys' entries get added, the average size of 
a bucket (the average length of the linked lists) never exceeds
the given `loadFactor`. We'll talk more about this when we
discuss *rehashing* below. It returns a pointer to that
`freq::dict` struct.

I've written the code for `build` already. The remaining three you
need to write are:

• `increment(D,k)`: this increments count for a word `k` that was
processed in the text, within the dictionary of counts `D`.  If it is
the first occurrence of `k`, then a new entry for `k` gets added
to `D`.

• `getCount(D,k)`: this gives back the integer count stored for the word
`k` in the dictionary `D`.  If there's no entry for `k` in `D`, it gives
back the value 0.

• `dumpAndDestroy(D,sizep)`: this builds and returns a pointer to
a new array of word counts (as structs of type `entry`), and meanwhile
gives back all of `D`'s storage to the heap.  In other words, it copies
all the entries of `D` into a new array, and gives back the array.
It then calls `delete` on all the structs and arrays that are the
heap-allocated data of `D`. The entries should be "dumped" into that
new array in order of decreasing count.  Also, to communicate the
size of that array back to `main`, this function gets called with
a pointer `sizep` to an `int`. 

There is, in addition to these three functions, a fourth function
that you must write called `rehash`. It is described below.

**Rehashing to maintain a load factor**  

I hope you can convince yourself that a bucket hash table makes a
search for entries quite a bit more efficient than that of a single
linked list. We use the hash function to go to a particular bucket
responsible for holding a key. If the hash function is defined well
enough then, in general, we expect that the keys stored in the
dictionary will be (roughly) evenly distributed amongst the buckets.
That means that we only look at a *1/numBuckets* fraction of the total
number of entries when we scan through a bucket's linked list.

To keep good performance, a hash table tracks its "load factor" which
is the total number of entries divided by the number of buckets.  This
gives the average size of the linked lists.  When the load factor gets
too high, search performance suffers. One trick is to *rehash* the
table. This involves the following:

1. Building a new array with empty buckets, roughly double the
   size (a prime number bigger than twice the number of 
   buckets.

2. Going through each entry of each bucket of the old array,
   moving it into the correct bucket of the new array (note
   that this involves recomputing a `hashValue` with this
   larger size).

3. Replacing the old array with the new array.

4. Deleting the old array's heap storage, giving it back to the heap.
       
This should all be done within the `rehash` function.

So, putting this all together, when `increment` is called on a key `k`
to update a dictionary `D`, there may not be an entry for `k` in `D`.
Before adding `k`, we check to see if its addition makes `D` exceed
its `loadFactor`. If so, then we `rehash(D)` to make its array be
at least doubled in size, making sure we put in all the exisiting
enries for `D`. And then we add an entry for `k`.

---

### Part 2: text synthesis with `chats`

The coding work described just above pays attention to single word
statistics in a text. Here, we instead consider runs of words in texts.
We process a text and pay attention to its *bigrams*, each consecutive
pairs of words in the text, and also its *trigrams*, each consecutive
triple of words in the text.

Consider this excerpt from the *Odyssey*:

    "So I spake, and quickly they hearkened to my words. But of Scylla
    I told them nothing more, a bane none might deal with, lest haply
    my company should cease from rowing for fear, and hide them in the
    hold. In that same hour I suffered myself to forget the hard
    behest of Circe, in that she bade me in nowise be armed; but I did
    on my glorious harness and caught up two long lances in my hands,
    and went on the decking of the prow, for thence methought that
    Scylla of the rock would first be seen, who was to bring woe on my
    company. Yet could I not spy her anywhere, and my eyes waxed weary
    for gazing all about toward the darkness of the rock."

We have the sequence of bigrams:

    so i
    i spoke
    spoke and
    and quickly
    ...

And we have the sequence of trigrams:
    
    so i spoke
    i spoke and
    spoke and quickly
    and quickly they
    quickly they hearkened
    ...

Among the bigrams we see that several start with "to", namely

    to my
    to bring
    to forget

And among the trigrams we see that two start with the bigram "on my"

    on my glorious
    on my company

We could say that the word "to" has two *followers* in that excerpt:
"my", "bring", and "forget". We could also say that the bigram "on my"
has two followers "glorious" and "company."

If we examine a much larger text, then we can get a much richer collection
of bigrams and trigrams. Furthermore, there will be a wealth of followers
of words and bigrams.

Write the code for `chats.cc`, a program that processes lines of text
and builds a dictionary of bigram and trigram patterns it sees in that
text. More specifically, each entry in that dictionary has as its
search key either a word (like "to") or a bigram (like "on my"). And
then each key's associated value is a *collection* of ist
followers. So, processing the text in the file `odyssey.txt` included
in this folder, we'd get a dictionary that included these entries:

    to: the, Erebus, her, Scylla's, mourn, my, forget, bring, ...
    on my: company, men

In processing the text in this way, you should also pay attention to
the "stoppers". These are the punctuation marks ".", "?", and "?" that
end English sentences and get treated as words by our code. Words that
follow stoppers can be seen as possible starts to sentences. So, looking
up the word "." in our bigram/trigram dictionary of `odyssey.txt`, 
we'd find the entry

    .: for, and, not, her, up, thereby, verily, up, but, ...

because these are the first words of all sentences following a 
period in that file. We could, furthermore, artificially treat
the first line in any text as having followed a sentence ending with
a period. That file starts with "Lady Circe spoke unto me..." so
we could instead have the dictionary entry

    .: lady, for, and, not, her, up, thereby, verily, up, but, ...

for the stopper word ".". We'd also see that, when "and" starts
a sentence, that it is followed by several words. We have the
bigram entry

    . and: in, therein, thereon, looking, there, the, 

Once this dictionary of word followers and bigram followers is 
built, we now have a way of generating semi-gibberish! I'll
call this gibberish a *chat.*

To start a chat, we pick a random word that's a follower of "." in
our dictionary. Maybe that's "and." To pick the next word, we look
up the bigram ". and" in our dictionary, and pick one of its random
followers. Maybe we choose "in". And so then we work with the bigram
"and in" and see that it has the follower "the". Continuing we see
that "in the" has a bunch of followers, so we pick one at ranodm.
And so forth. This process might then give us this paragraph:

    and in the dread death struggle. as often as she belched it
    forth and thrice a day she sucks the water for thrice a day
    she sucks the water for thrice a day she sucks the water for
    thrice a day she sucks the water for thrice a day she sucks
    the water for thrice a day she sucks it down in terrible
    wise sucked down the salt sea water within she was all plain
    to see through her troubled deeps and overhead the spray
    fell on the decking of the sea. for on the other part are
    two rocks whereof the deep voiced amphitrite feeds countless
    flocks. not with an arrow across. but that other cliff
    odysseus thou shalt note lying lower hard by the first thou
    couldest send an arrow from a bow might a man whom she hath
    snatched from out the paths of the prow for thence methought
    that scylla of the cliff. yet could i not spy her anywhere
    and my eyes waxed weary for gazing all about toward the
    darkness of the rock for dolphins or sea dogs or whatso
    greater beast she may anywhere take whereof the one hand lay
    scylla and on the other part are two rocks whereof the one
    reaches with sharp peak to the wide heaven and a dark cloud
    encompasses it this never streams away and there is no clear
    air about the peak neither in summer nor in harvest tide.

It's not great, but it almost reads as something poignant!
Well, sort of.

But then applying the same technique to a longer text, things
get maybe *a little* better. Here's a random result that comes from
training such a process with `hundred_years.txt`:

    aureliano segundo saw the starting point of her dense and
    prolonged silences that had turned him on the edge of town
    the pleasure of seeing us weep. somebody is coming he told
    him that for the first roll of sweaty papers from under the
    friendly eye of ursula's knowing it or thinking about
    mauricio babilonia under the dark shapes of the anxious
    efforts of his hand. a broken down yellow train and that his
    wife who was not the impossibility of finding anyone to
    guess distances in the usual gothic letters to her that
    aureliano mentioned the matter was put under the mosquito
    netting that sleepwalker whom she could not succeed however
    in incorporating fernanda into the abyss of greatness. it
    was stuck to the heroism of his withdrawn self and made the
    earth i'll convince you. then he came back from traveling
    around the room. suddenly during the months that at that
    time also stumbled and had been carried off to pray. colonel
    roque carnicero which meant butcher. was all he could wait
    as long as they got their peso and twenty five cents. they
    were rebeca s name bringing back the acidity of the latter
    elicited the whole group tried to cure with the saints will
    let me undergo the indignity of dying in the only thing that
    she will produce milk and biscuits and would bathe him bit
    by bit as he faced the firing squad could be heard
    chattering and singing at the door.
   
In any case, write the code that processes a text and generates
a chat using this "random following word" process. It should
be the result of running a procedure `main` inside a file named
`chats.cc`.

You can write this code all from scratch, and however you choose.
I've provided some starting code that you are welcome to use.  In that
code, I assume there's a dictionary data structure `dict*` defined in
a namespace `gram`, in files `gram.cc` and `gram.hh`, and with an
interface of `get` and `add` calls for looking up a random follower of
a word/bigram (using `get`) and also a for adding followers of a
word/bigram to that dictionary (using `add`).  Complete that code or,
alternatively, write your own code as you see fit.

---

### What to hand in

Submit all the C++ source files you changed or added to make the
programs of these two parts work. Also, if you changed my provided
`Makefile`, include that as well. Finally, I'd like you to include
sample output of `stats` and `chats` for each of the sample `.txt`
files included in this folder. You can do that in the console by using
*output redirection*, which is similar to input redirection.
If, for example, you type

    ./stats < sonnets.txt > sonnets_stats.txt

then you will get a file with the statistics generated by the
run of `stats` on the file `sonnets.txt`. If instead you type
   
    ./chats < sonnets.txt > sonnets_chats.txt
   
then you will get a file of a random chat output by your `chats`
program, and trained on the file `sonnets.txt`.

If, also, you want to tell us a bit more about your work, and
document any other files, quirks, problems, or output of you
program, and also how to run it, include a file named
`DOC.md` that documents these aspects of your project.

