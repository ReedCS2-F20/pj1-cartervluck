CXX=g++
CXX_FLAGS=-g -std=c++11
#CXX_FLAGS=-g -std=c++11 -fsanitize=address -fsanitize=leak
.PHONY: all clean git
BRANCH=work
TARGETS=stats chats
SOURCES=stats.cc freq.cc freq.hh chats.cc gram.cc gram.hh
TEXTS=
COMMITS=Makefile $(SOURCES) $(TEXTS) DOC.md

all: $(TARGETS)

stats.o: freq.hh
stats.o: stats.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

freq.o: freq.hh
freq.o: freq.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

stats: stats.o freq.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

chats.o: gram.hh
chats.o: chats.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

gram.o: gram.hh
gram.o: gram.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

chats: chats.o gram.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

git: $(COMMITS)
	git add $(COMMITS)
	git commit -m "Completed Project 1."
	git push origin $(BRANCH)

clean:
	rm -f *.o *~ a.out core $(TARGETS)
