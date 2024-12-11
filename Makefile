# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra

# OpenSSL flags for hash_object.cpp
OPENSSL_FLAGS = -lssl -lcrypto
ZLIB_FLAGS = -lz

# Directories
SRC_DIR = .
CMD_DIR = commands
UTILS_DIR = utils
BUILD_DIR = build

# Source files
SRCS = 	$(SRC_DIR)/main.cpp \
       	$(CMD_DIR)/init.cpp \
       	$(CMD_DIR)/hash_object.cpp \
       	$(CMD_DIR)/cat_file.cpp \
       	$(CMD_DIR)/write_tree.cpp \
       	$(CMD_DIR)/ls_tree.cpp \
       	$(CMD_DIR)/add.cpp \
       	$(CMD_DIR)/commit.cpp \
       	$(CMD_DIR)/log.cpp \
       	$(CMD_DIR)/checkout.cpp \
		$(UTILS_DIR)/logger.cpp \
		$(UTILS_DIR)/blob_creator.cpp

# Object files
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Output executable
TARGET = mygit

# Default rule to build the project
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(OPENSSL_FLAGS) $(ZLIB_FLAGS)

# Rule to compile each source file to an object file
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(if $(filter $(CMD_DIR)/hash_object.cpp,$<),$(OPENSSL_FLAGS),$(ZLIB_FLAGS)) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/$(CMD_DIR) $(BUILD_DIR)/$(UTILS_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
