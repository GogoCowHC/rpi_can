# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -I MyLib -MMD -MP

# Source files
SRCS = main.cpp $(wildcard MyLib/*.cpp)

# Object files
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

# Output file
TARGET = main

all: $(TARGET)

# Build target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

# Automatically include dependency files
-include $(DEPS)