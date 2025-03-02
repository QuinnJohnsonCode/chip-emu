CXX=g++
CXXFLAGS=-std=c++23 -Wall -Werror -Wextra
RM=rm -f
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs) -lSDL2 -lSDL2_mixer

OBJDIR=obj
SRCS=main.cpp interpreter.cpp display_manager.cpp
OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
HEADERS=interpreter.h font.h display_manager.h

.PHONY: all clean distclean

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@.out $(OBJS) $(LDLIBS)

$(OBJDIR)/%.o: %.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR)/*.o *.out

distclean: clean
	$(RM) -r $(OBJDIR) main