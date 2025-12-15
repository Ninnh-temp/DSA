# Makefile for Student Management System
# Single-file implementation

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = student_management
SOURCES = main.cpp

# Default target
all: $(TARGET)

# Build the executable (single file compilation)
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
