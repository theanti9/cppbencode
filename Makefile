CXX = g++
CXXFLAGS = -O2 -W -Wall -g
OBJS = bencode.o

all: $(OBJS)
	cd example && $(MAKE)
	cd tests && $(MAKE)

clean:
	rm -rf $(OBJS)
