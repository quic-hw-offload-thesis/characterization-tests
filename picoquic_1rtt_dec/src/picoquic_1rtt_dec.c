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
 * "quic-spin-bit" : "0b1",
 * "quic-reserved" : "0b0",
 * "quic-key-phase" : "0b0",
 * "quic-packet-nr-len" : "2",
 * "quic-dcid" : "f3dec1bb98d92f7402e898cec879542cbd28234c",
 * "quic-packet-nr" : "0xc6b0c9",
 * "quic-hp-key" : "acfa8bcf0b477016ce23e75d5db6182f",
 * "quic-pp-key" : "e80afdae24beb1764018f18e094a93ed",
 * "quic-iv" : "d448041afa37c0322fe18eee",
 * "quic-plaintext-payload" : "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
 */
uint8_t msg[] = "\x6d\xf3\xde\xc1\xbb\x98\xd9\x2f\x74\x02\xe8\x98\xce\xc8\x79\x54\x2c\xbd\x28\x23\x4c\x76\xb8\x64\x5e\xe1\xee\xe1\xc3\x7d\xb9\xc7\x79\x11\x2a\xe8\xbb\x5a\xfa\xaa\x6c\xbb\x71\xd3\xf0\xcd\x29\xbe\x3a\xb0\x40\x14\x65\x86\x30\xe8\xc0\x2b\xfc\x75\xb5\xe3\x22\xc2\xa4\x59\x32\x17\x02\xc2\x98\x52\x07\x94\x3d\x2b\x4b\xef\xbf\x29\x78\x7f\x2d\xdd\x2b\x74\x9d";

// Pointers to in en out buffer
int buffer_size = 1024;
uint8_t *bytes_in;
uint8_t *bytes_out;

int main(int argc, char **argv)
{
    // Set initial exit code to success
    int exit_code = 0;

    // Allocate memory for input buffer
    bytes_in = malloc(buffer_size);
    memset(bytes_in, 0, buffer_size);

    // Read data input file to buffer
    memcpy(bytes_in, &msg, sizeof(msg)); // Fixme: Set fixed msg to input buffer for debug, should be read from file for final test

    // Create picoquic context
    // picoquic_quic_t *quic = create_picoquic_context();
    picoquic_quic_t *quic = NULL;

    // Todo: Add connections to context

    int length = sizeof(msg);   // Fixme: Set length to msg len for debug, should be set to len of nth packet for final test
    int packet_length = length; // Parse one packet at a time

    // Create parameters for `picoquic_parse_header_and_decrypt`
    // Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2135
    picoquic_cnx_t *cnx = NULL;
    picoquic_packet_header ph;
    int new_context_created = 0;
    picoquic_stream_data_node_t *decrypted_data = picoquic_stream_data_node_alloc(quic); // BUG: Segfault happens here since `quic` is `NULL`
    size_t consumed = 0;                                                                 // Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2505
    uint64_t current_time = picoquic_current_time();                                     // Current time is calculated as last before function call

    //  Parse & decrypt bytes_in into bytes_out
    picoquic_parse_header_and_decrypt(
        quic,                // quic: Picoquic context
        bytes_in,            // bytes: Input bytes
        length,              // length: Length of input bytes
        packet_length,       // packet_length: Total packet length
        NULL,                // addr_from:
        current_time,        // current_time: Current time
        decrypted_data,      // decrypted_data: picoquic_stream_data_node_t to fill
        &ph,                 // ph: picoquic_packet_header to fill
        &cnx,                // pcnx:
        &consumed,           // consumed: Amount of consumed bytes?
        &new_context_created // new_context_created: 0 if there isn't a new context created?
    );
    bytes_out = decrypted_data->data; // Fixme: Set pointer for debug, should append for final test

    printf("picoquic_1rtt_dec has ran\n");

    // Cleanup
    free(bytes_in);
    picoquic_stream_data_node_recycle(decrypted_data); // Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2400
    exit(exit_code);
}

// /**
//  * @brief Create a picoquic context object
//  *
//  * @return picoquic_quic_t* picoquic context
//  */
// picoquic_quic_t *create_picoquic_context()
// {
//     picoquic_quic_t *quic_context = NULL;
//     uint64_t current_time = picoquic_current_time();
//     // Todo: create and configure quic context
//     return NULL;
// }

// /**
//  * @brief Add new connection to context
//  *
//  * @param context Context which to add the new connection to
//  * @param dcid Dcid of new connection
//  * @return picoquic_cnx_t* new connection
//  */
// picoquic_cnx_t *add_connection_to_context(picoquic_quic_t *context, picoquic_connection_id_t *dcid) // Todo: add keys to connection creation function
// {
//     // Todo: create connection and add to context
//     return NULL;
// }
