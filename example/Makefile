CXX = g++
CXXFLAGS = -O2 -W -Wall -g -I../ -L../
OBJS = example.o utils.o

example: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -lcppbencode

clean:
	rm -rf $(OBJS)
