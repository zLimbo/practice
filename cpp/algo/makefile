SOURCE := $(wildcard *.cpp)
EXEC := $(SOURCE:.cpp=)
OBJ := $(SOURCE:.cpp=.o)
CC := g++ -std=c++11 -g

$(EXEC): %: %.cpp
	$(CC) -o $@ $^