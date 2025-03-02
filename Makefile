# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -arch arm64 # Use C++17, enable warnings, optimize

# Executable and source files
TARGET = primes
SRC = primes.cpp

# Default target
all: $(TARGET)

# Compile the C++ program
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up generated files
clean:
	rm -f $(TARGET)
