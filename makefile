# ------------------------------------------------------------------------------
# Master's thesis: Hardware/software co-design for the new QUIC network protocol
# ------------------------------------------------------------------------------
# Characterization tests
#
# File: makefile (makefile)
# By: Lowie Deferme (UHasselt/KULeuven - FIIW)
# On: 05 July 2022
# ------------------------------------------------------------------------------

# Define directories with tests
TEST_DIRS := ./picoquic_1rtt_dec

LIB_DIR := ./libs

TARGET_DIR := ./target
BINARIES := $(wildcard $(TARGET_DIR)/*)

.PONY: all

all: bin clean run

bin:
	for dir in $(TEST_DIRS); \
		do $(MAKE) -C $$dir bin; \
	done

clean:
	for dir in $(TEST_DIRS); \
		do $(MAKE) -C $$dir clean; \
	done

run: 
	for bin in $(BINARIES); \
		do $$bin; \
	done

lib:
	$(MAKE) -C $(LIB_DIR)/picotls clean
	$(MAKE) -C $(LIB_DIR)/picotls
	$(MAKE) -C $(LIB_DIR)/picoquic  clean
	$(MAKE) -C $(LIB_DIR)/picoquic

