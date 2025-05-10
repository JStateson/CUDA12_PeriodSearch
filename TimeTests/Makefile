# Define compiler and flags
BUILD_DIR = build
CXX = g++
CXXFLAGS = -march=x86-64 -O3 -std=c++2a -w -s -L.

TARGET =$(BUILD_DIR)/TimeTestsLinux
SRC = TimeTests.cpp
OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

# Default target to build the executable
all: $(BUILD_DIR) $(TARGET)

# Rule to create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(TARGET): $(OBJ) $(BUILD_DIR)/libstdc++.a
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(BUILD_DIR)/libstdc++.a -lm



# Rule to build object files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to create symbolic link for libstdc++.a
$(BUILD_DIR)/libstdc++.a:
	@mkdir -p $(BUILD_DIR)
	ln -s `g++ -print-file-name=libstdc++.a` $(BUILD_DIR)/libstdc++.a

# Clean rule to remove compiled files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
