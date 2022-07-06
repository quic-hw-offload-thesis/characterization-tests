// ------------------------------------------------------------------------------
// Master's thesis: Hardware/software co-design for the new QUIC network protocol
// ------------------------------------------------------------------------------
// Characterization tests
//
// File: picoquic_1rtt_dec.c (c)
// By: Lowie Deferme (UHasselt/KULeuven - FIIW)
// On: 04 July 2022
// ------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

#include "../../libs/picoquic/picoquic/picoquic.h"
#include "../../libs/picoquic/picoquic/picoquic_utils.h"
#include "../../libs/picoquic/picoquic/picosocks.h"

/**
 * @brief Print current time.
 * @details For testing out picoquic binding
 * @return uint64_t Current time
 */
uint64_t print_curr_time()
{
    uint64_t current_time = 0;
    current_time = picoquic_current_time();
    printf("Current time: %llu\n", current_time);
    return current_time;
}

int main(int argc, char **argv)
{
    int exit_code = 0;
    print_curr_time();
    printf("picoquic_1rtt_dec has ran\n");
    exit(exit_code);
}
