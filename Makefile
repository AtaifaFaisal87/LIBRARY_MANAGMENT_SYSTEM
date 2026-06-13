# Library Management System Makefile
# Compile with: make
# Clean with: make clean

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

SRCS = main.cpp Utils.cpp Library.cpp User.cpp Librarian.cpp Authentication.cpp FileIO.cpp Fines.cpp Statics.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = library_system

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.dat

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
