# -------------------------------------------------
# Toolchain
# -------------------------------------------------
CC      := gcc
AR      := ar

# -------------------------------------------------
# Directories
# -------------------------------------------------
SRC_DIR     := src
INC_DIR     := include
TEST_DIR    := tests
BIN_DIR     := bin
DIST_DIR    := dist

# -------------------------------------------------
# Flags
# -------------------------------------------------
CFLAGS_COMMON  := -I$(INC_DIR) -Wall
CFLAGS_RELEASE := $(CFLAGS_COMMON) -Werror
CFLAGS_DEBUG   := $(CFLAGS_COMMON) -g

# Default build flags
CFLAGS := $(CFLAGS_RELEASE)

# -------------------------------------------------
# Library
# -------------------------------------------------
LIB_NAME := libhcip.a
LIB_PATH := $(BIN_DIR)/$(LIB_NAME)

# -------------------------------------------------
# Sources
# -------------------------------------------------
LIB_SRCS := \
    $(SRC_DIR)/parser.c \
    $(SRC_DIR)/spectable.c

LIB_OBJS := $(LIB_SRCS:$(SRC_DIR)/%.c=$(DIST_DIR)/%.o)

TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(TEST_SRCS:$(TEST_DIR)/%.c=$(BIN_DIR)/%)

# -------------------------------------------------
# Targets
# -------------------------------------------------
.PHONY: all debug clean dirs

all: dirs $(LIB_PATH) $(TEST_BINS)

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: clean all

dirs:
	@mkdir -p $(BIN_DIR) $(DIST_DIR)

# -------------------------------------------------
# Static library
# -------------------------------------------------
$(LIB_PATH): $(LIB_OBJS)
	$(AR) rcs $@ $^

# -------------------------------------------------
# Object rules (library)
# -------------------------------------------------
$(DIST_DIR)/parser.o: $(SRC_DIR)/parser.c $(SRC_DIR)/parser.h $(INC_DIR)/HCIp.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIST_DIR)/spectable.o: $(SRC_DIR)/spectable.c $(SRC_DIR)/parser.h $(INC_DIR)/HCIp.h
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------------------------
# Test binaries (each has its own main)
# -------------------------------------------------
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(LIB_PATH) $(INC_DIR)/HCIp.h
	$(CC) $(CFLAGS) $< -L$(BIN_DIR) -lhcip -o $@

# -------------------------------------------------
# Cleanup
# -------------------------------------------------
clean:
	rm -rf $(BIN_DIR) $(DIST_DIR)
