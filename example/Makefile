CXX = g++
CXXFLAGS = -O2 -W -Wall -g -I../
OBJS = ../bencode.o example.o utils.o

example: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -rf $(OBJS)
