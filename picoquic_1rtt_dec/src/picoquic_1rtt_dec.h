// ------------------------------------------------------------------------------
// Master's thesis: Hardware/software co-design for the new QUIC network protocol
// ------------------------------------------------------------------------------
// Characterization tests
//
// File: picoquic_1rtt_dec.h (c)
// By: Lowie Deferme (UHasselt/KULeuven - FIIW)
// On: 05 July 2022
// ------------------------------------------------------------------------------

#ifndef PICOQUIC_DEC
#define PICOQUIC_DEC

#include <stdlib.h>

#include "../../libs/picoquic/picoquic/picoquic_internal.h"

/**
 * @brief Print a array's value and end with newline
 *
 * @param array Buffer to print
 * @param array_len Length of array to print
 * @return int Zero if success
 */
int print_byte_array(uint8_t *array, int array_len);

/**
 * @brief Convert ptype to string presentation
 *
 * @param ptype ptype to convert
 * @param str Output string representation
 * @return int Zero if success
 */
int ptype_to_string(picoquic_packet_type_enum ptype, char **str);

#endif
