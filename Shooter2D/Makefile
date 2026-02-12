# Simple Makefile for Linux
CXX = g++
CXXFLAGS = -std=c++14 -Wall -O2
LDFLAGS = -lSDL2 -lm

TARGET = SpaceShooter
SOURCES = main.cpp Game.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
