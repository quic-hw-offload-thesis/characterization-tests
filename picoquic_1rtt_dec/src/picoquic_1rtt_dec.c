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
#include "../../libs/picoquic/picoquic/picoquic_internal.h"
#include "../../libs/picoquic/picoquic/picoquic_utils.h"

/**
 * @brief Create a picoquic context object
 *
 * @return picoquic_quic_t* picoquic context
 */
picoquic_quic_t *create_picoquic_context()
{
    picoquic_quic_t *quic_context = NULL;
    uint64_t current_time = picoquic_current_time();
    // Todo: create and configure quic context
    return NULL;
}

/**
 * @brief Add new connection to context
 *
 * @param context Context which to add the new connection to
 * @param dcid Dcid of new connection
 * @return picoquic_cnx_t* new connection
 */
picoquic_cnx_t *add_connection_to_context(picoquic_quic_t *context, picoquic_connection_id_t *dcid) // Todo: add keys to connection creation function
{
    // Todo: create connection and add to context
    return NULL;
}

// Pointers to in en out buffer
int buffer_size = 2048;
uint8_t *bytes_in;
uint8_t *bytes_out;

int main(int argc, char **argv)
{
    // Set initial exit code to success
    int exit_code = 0;

    // Allocate memory for in/out buffers
    bytes_in = malloc(buffer_size);
    bytes_out = malloc(buffer_size);
    memset(bytes_in, 0, buffer_size);
    memset(bytes_out, 0, buffer_size);

    // Todo: Read data input file to buffer

    // Create picoquic context
    picoquic_quic_t *quic = create_picoquic_context();

    // Todo: Add connections to context

    // Create parameters for `picoquic_parse_header_and_decrypt`
    uint64_t current_time = picoquic_current_time();
    picoquic_stream_data_node_t *decrypted_data = picoquic_stream_data_node_alloc(quic);

    // Parse & decrypt bytes_in
    picoquic_parse_header_and_decrypt(
        quic,                    // Picoquic context (quic)
        bytes_in,                // (bytes)
        0,                       // (length)
        0,                       // (packet_length)
        NULL,                    // (addr_from)
        picoquic_current_time(), // Current time (current_time)
        decrypted_data,          // (decrypted_data)
        NULL,                    // Packet header (ph)
        NULL,                    // (pcnx)
        NULL,                    // (consumed)
        NULL                     // (new_context_created)
    );

    printf("picoquic_1rtt_dec has ran\n");

    // Cleanup
    free(bytes_in);
    free(bytes_out);
    exit(exit_code);
}
