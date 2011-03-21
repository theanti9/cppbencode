CXX = g++
CXXFLAGS = -O2 -W -Wall -g -I../ -L../
OBJS = tests.o

tests: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -lcppbencode

clean:
	rm -rf $(OBJS)
