SOURCE := $(wildcard *.cpp)
EXEC := $(SOURCE:.cpp=)
OBJ := $(SOURCE:.cpp=.o)
CC := g++ -std=c++11

$(EXEC): %: %.cpp
	$(CC) -o $@ $^