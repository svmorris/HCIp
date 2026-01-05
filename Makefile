# Toolchain
CC := cc

# Directories
SRC_DIR   := src
INC_DIR   := include
UTIL_DIR  := util
TEST_DIR  := tests
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
BIN_DIR   := $(BUILD_DIR)/bin

# Core library sources
LIB_SRCS := \
	$(SRC_DIR)/parser.c \
	$(SRC_DIR)/events.c

LIB_OBJS := \
	$(OBJ_DIR)/parser.o \
	$(OBJ_DIR)/events.o

# Util (tests only)
UTIL_SRCS := $(wildcard $(UTIL_DIR)/*.c)
UTIL_OBJS := $(patsubst $(UTIL_DIR)/%.c,$(OBJ_DIR)/util_%.o,$(UTIL_SRCS))

# Tests
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

# Include paths
CPPFLAGS := -I$(INC_DIR) -I$(SRC_DIR) -I$(UTIL_DIR)

# Warnings
WARN_FLAGS := -Wall -Wextra -Wpedantic

# Release / hardened
CFLAGS_RELEASE := \
	$(WARN_FLAGS) \
	-O2 \
	-fstack-protector-strong \
	-D_FORTIFY_SOURCE=2 \
	-fPIE

LDFLAGS_RELEASE := \
	-pie \
	-Wl,-z,relro \
	-Wl,-z,now

# Debug
CFLAGS_DEBUG := \
	$(WARN_FLAGS) \
	-g \
	-O0 \
	-fno-omit-frame-pointer \
	-DDEBUG

# Default: release
.PHONY: all
all: CFLAGS := $(CFLAGS_RELEASE)
all: LDFLAGS := $(LDFLAGS_RELEASE)
all: dirs $(TEST_BINS)

# Debug build
.PHONY: debug
debug: CFLAGS := $(CFLAGS_DEBUG)
debug: LDFLAGS :=
debug: dirs $(TEST_BINS)

# Create build directories
.PHONY: dirs
dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Core objects
$(OBJ_DIR)/parser.o: $(SRC_DIR)/parser.c \
                     $(SRC_DIR)/parser.h \
                     $(SRC_DIR)/events.h \
                     $(INC_DIR)/hcip.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/events.o: $(SRC_DIR)/events.c \
                     $(SRC_DIR)/events.h \
                     $(SRC_DIR)/parser.h \
                     $(INC_DIR)/hcip.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Util objects (tests only)
$(OBJ_DIR)/util_%.o: $(UTIL_DIR)/%.c $(UTIL_DIR)/%.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Test binaries (link core + util)
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(LIB_OBJS) $(UTIL_OBJS) $(INC_DIR)/hcip.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB_OBJS) $(UTIL_OBJS) $(LDFLAGS) -o $@

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
