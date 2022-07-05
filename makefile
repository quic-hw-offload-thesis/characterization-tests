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
TEST_DIRS := ./picoquic_1rtt_dec ./soc_1rtt_dec

TARGET_DIR := ./target
BINARIES := $(wildcard $(TARGET_DIR)/*.bin)

.PONY: all

all: bin run

bin:
	for dir in $(TEST_DIRS); \
		do $(MAKE) -C $$dir bin; \
	done

run: 
	for bin in $(BINARIES); \
		do $$bin; \
	done