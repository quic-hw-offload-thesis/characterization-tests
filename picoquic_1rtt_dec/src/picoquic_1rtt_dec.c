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
picoquic_cnx_t *add_connection_to_context(picoquic_quic_t *context, picoquic_connection_id_t *dcid)
{
    // Todo: create connection and add to context
    return NULL;
}

int main(int argc, char **argv)
{
    int exit_code = 0;

    picoquic_quic_t *quic_context = create_picoquic_context();
    uint8_t *bytes;

    // picoquic_parse_header_and_decrypt(
    //     quic_context,

    // );

    printf("picoquic_1rtt_dec has ran\n");
    exit(exit_code);
}
