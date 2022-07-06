# ------------------------------------------------------------------------------
# Master's thesis: Hardware/software co-design for the new QUIC network protocol
# ------------------------------------------------------------------------------
# Characterization tests
#
# File: makefile (makefile)
# By: Lowie Deferme (UHasselt/KULeuven - FIIW)
# On: 05 July 2022
# ------------------------------------------------------------------------------

# Define final executable
BINARY = soc_1rtt_dec

# Define build, target and source dir
BUILD_DIR := ./build
TARGET_DIR := ../target
SOURCE_DIR := ./src

# Define compiler and options
CC := gcc
CFLAGS := -g
# LDFLAGS := 

# Define utilities
RM = rm -rf
MKDIR = mkdir -p

SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

.PHONY: all
all: bin clean run

bin: $(info [BUILD]) $(BINARY)

$(OBJECTS) : $(SOURCES) | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BINARY) : $(OBJECTS) | $(TARGET_DIR)
	$(CC) $< -o $(TARGET_DIR)/$@

$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

$(TARGET_DIR):
	$(MKDIR) $(TARGET_DIR)

# Remove all object files
clean: | $(BUILD_DIR)
	$(info [CLEAN])
	$(RM) $(BUILD_DIR)/*.o

# Run the final binary
run:
	$(info [RUN])
	$(TARGET_DIR)/$(BINARY)
