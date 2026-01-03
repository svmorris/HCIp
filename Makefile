# Toolchain
CC := cc

# Directories
SRC_DIR   := src
INC_DIR   := include
TEST_DIR  := tests
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
BIN_DIR   := $(BUILD_DIR)/bin

# Sources
LIB_SRC := $(SRC_DIR)/parser.c
LIB_OBJ := $(OBJ_DIR)/parser.o

TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

# Include paths
CPPFLAGS := -I$(INC_DIR) -I$(SRC_DIR)

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

# Parser object (depends on BOTH headers)
$(LIB_OBJ): $(LIB_SRC) $(SRC_DIR)/parser.h $(INC_DIR)/hcip.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Test binaries (tests include only hcip.h)
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(LIB_OBJ) $(INC_DIR)/hcip.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB_OBJ) $(LDFLAGS) -o $@

# Cle
