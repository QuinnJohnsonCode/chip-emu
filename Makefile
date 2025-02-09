CXX=g++
CXXFLAGS=-std=c++23 -Wall -Werror -Wextra
RM=rm -f
LDFLAGS=-g $(root-config --ldflags)
LDLIBS=$(root-config --libs)

SRCS=chip.cpp
OBJS=$(SRCS:.cpp=.o)
HEADERS=

.PHONY: all clean distclean

all: chip

chip: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@.out $(OBJS) $(LDLIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) main