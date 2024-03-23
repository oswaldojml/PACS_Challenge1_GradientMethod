CXX      ?= g++
CXXFLAGS ?= -std=c++20

SRCS= $(wildcard *.cpp)
OBJS= $(SRCS:.cpp=.o)
EXEC= main
.phony= clean 
.DEFAULT_GOAL = all
all: $(EXEC)

$(OBJS): $(SRCS) 
	$(CXX) -c $(CXXFLAGS)  $(SRCS)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

clean:
	$(RM) *.o
	$(RM) $(EXEC)