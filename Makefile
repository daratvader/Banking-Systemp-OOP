CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = $(wildcard src/*.cpp) main.cpp
OBJ      = $(SRC:.cpp=.o)
TARGET   = banking

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: all
	./$(TARGET)

demo: all
	./$(TARGET) --demo

.PHONY: all clean run demo
