CXX = g++
CXXFLAGS = -O2 -W -Wall -g -I../
OBJS = ../bencode.o tests.o

tests: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -rf $(OBJS)
