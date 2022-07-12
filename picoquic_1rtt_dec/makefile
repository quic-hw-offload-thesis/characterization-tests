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
BINARY = picoquic_1rtt_dec

# Define build, target and source dir
BUILD_DIR := ./build
TARGET_DIR := ../target
SOURCE_DIR := ./src

# Link dependencies
# Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/sample/README.md
LINK_DEP := ../libs/picoquic/libpicoquic-core.a ../libs/picotls/libpicotls-core.a ../libs/picotls/libpicotls-minicrypto.a ../libs/picotls/libpicotls-openssl.a ../libs/picotls/libpicotls-fusion.a

# Define compiler and options
CC := gcc
CFLAGS := -g
LDFLAGS := -lssl -lcrypto -lpthread

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
	$(CC) $< $(LINK_DEP) $(LDFLAGS) -o $(TARGET_DIR)/$@

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
