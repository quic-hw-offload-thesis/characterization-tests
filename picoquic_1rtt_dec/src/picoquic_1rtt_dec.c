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
#include <sys/socket.h>

#include "../../libs/picoquic/picoquic/picoquic.h"
#include "../../libs/picoquic/picoquic/picoquic_internal.h"
#include "../../libs/picoquic/picoquic/picoquic_utils.h"
#include "../../libs/picoquic/picoquic/tls_api.h"

#include "picoquic_1rtt_dec.h"

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
size_t msg_len = sizeof(msg) - 1; // remove the null terminator

// global pointers to input and output buffer
int buffer_size = PICOQUIC_MAX_PACKET_SIZE;
uint8_t *bytes_in;
uint8_t *bytes_out;

int main(int argc, char **argv)
{
    // Set initial exit code to success
    int exit_code = 0;

    // Set object's pointer to nullpointer initially
    picoquic_stream_data_node_t *decrypted_data = NULL;

    // Allocate memory for input buffer
    bytes_in = malloc(buffer_size);
    memset(bytes_in, 0, buffer_size);

    // Read data input file to buffer
    memcpy(bytes_in, &msg, msg_len); // Fixme: Set fixed msg to input buffer for debug, should be read from file for final test

    /*****************************************************
     *              Create picoquic context              *
     *****************************************************/
    picoquic_quic_t *quic = picoquic_create( // Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquictest/parseheadertest.c#L568
        8,                                   // uint32_t max_nb_connections
        NULL,                                // char const* cert_file_name,
        NULL,                                // char const* key_file_name,
        NULL,                                // char const * cert_root_file_name
        NULL,                                // char const* default_alpn
        NULL,                                // picoquic_stream_data_cb_fn default_callback_fn
        NULL,                                // void* default_callback_ctx
        NULL,                                // picoquic_connection_id_cb_fn cnx_id_callback
        NULL,                                // void* cnx_id_callback_data
        NULL,                                // uint8_t reset_seed[PICOQUIC_RESET_SECRET_SIZE]
        picoquic_current_time(),             // uint64_t current_time
        NULL,                                // uint64_t* p_simulated_time
        NULL,                                // char const* ticket_file_name
        NULL,                                // const uint8_t* ticket_encryption_key
        0                                    // size_t ticket_encryption_key_length
    );

    if (quic != NULL)
    {
        // Set context's settings
        quic->local_cnxid_length = 20;

        /*****************************************************
         *            Create picoquic connection             *
         *****************************************************/
        printf("\n[DEBUG] Create picoquic connection:\n");
        printf("------- ---------------------------\n");

        // Create socket address for this connection
        // Source: Beej’s Guide to Network Programming
        struct sockaddr_in sa; // IPv4
        memset(&sa, 0, sizeof sa);
        sa.sin_family = AF_INET;
        inet_pton(AF_INET, "10.25.135.108", &(sa.sin_addr));
        sa.sin_port = 4434;

        // Create connection
        picoquic_cnx_t *connection = picoquic_create_cnx(
            quic,                        // picoquic_quic_t* quic
            picoquic_null_connection_id, // picoquic_connection_id_t initial_cnx_id
            picoquic_null_connection_id, // picoquic_connection_id_t remote_cnx_id
            (struct sockaddr *)&sa,      // const struct sockaddr* addr_to
            0,                           // uint64_t start_time
            0,                           // uint32_t preferred_version
            NULL,                        // char const* sni
            NULL,                        // char const* alpn
            0                            // char client_mode
        );

        // Create dcid and add to connection
        uint8_t dcid_id[] = "\xf3\xde\xc1\xbb\x98\xd9\x2f\x74\x02\xe8\x98\xce\xc8\x79\x54\x2c\xbd\x28\x23\x4c"; // Fixme: Use fixed dcid for debug, should be read from dcid file in final test
        picoquic_connection_id_t dcid = *((picoquic_connection_id_t *)(malloc(sizeof(picoquic_connection_id_t))));
        dcid.id_len = sizeof dcid_id - 1; // Subtract string terminator
        for (int i = 0; i < dcid.id_len; i++)
        {
            dcid.id[i] = dcid_id[i]; // Copy values since pointer ref may cause problems later
        }
        connection->path[0]->p_local_cnxid = picoquic_create_local_cnxid(connection, &dcid, 0);

        // Create and fill connection's crypto context
        // Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquictest/parseheadertest.c#L842
        char *prefix_label = picoquic_supported_versions[connection->version_index].tls_prefix_label;
        static const uint8_t secret[] = {
            0, 1,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        void *aead_context = picoquic_setup_test_aead_context(0, secret, prefix_label);
        void *pn_context = picoquic_pn_enc_create_for_test(secret, prefix_label);
        connection->crypto_context[3].aead_decrypt = aead_context;
        connection->crypto_context[3].pn_dec = pn_context;
        printf("[DEBUG] Crypto context setup: \n");
        printf("\t  Version index: %d\n", connection->version_index);
        printf("\t  Prefix label: %d\n", *prefix_label);

        // Fixme: Proof that connection is added to context is only for debug
        printf("[DEBUG] First path of first conn in quic context: \n");
        printf(" \t  Local CID: ");
        print_byte_array(quic->cnx_list[0].path[0]->p_local_cnxid->cnx_id.id, quic->cnx_list[0].path[0]->p_local_cnxid->cnx_id.id_len);
        printf("\t  Remote CID: ");
        print_byte_array(quic->cnx_list[0].path[0]->p_remote_cnxid->cnx_id.id, quic->cnx_list[0].path[0]->p_remote_cnxid->cnx_id.id_len);

        // Create parameters for `picoquic_parse_header_and_decrypt`
        decrypted_data = picoquic_stream_data_node_alloc(quic);
        if (decrypted_data != NULL)
        {
            int length = msg_len;       // Fixme: Set length to msg len for debug, should be set to len of nth packet for final test
            int packet_length = length; // Parse one packet at a time
            // Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2135
            picoquic_packet_header ph;
            picoquic_cnx_t *cnx = NULL;
            int new_context_created = 0;
            size_t consumed = 0;                             // Source: https://github.com/private-octopus/picoquic/blob/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2505
            uint64_t current_time = picoquic_current_time(); // Current time is calculated as last before function call

            //  Parse & decrypt bytes_in into bytes_out
            printf("\n[INFO] Calling picoquic_parse_header_and_decrypt:\n");
            printf("------ ------------------------------------------\n");
            printf("[DEBUG] Bytes in: ");
            // print_byte_array(bytes_in, buffer_size);
            print_byte_array(bytes_in, msg_len);
            printf("[DEBUG] Length: %d (in)\n", length);
            printf("[DEBUG] Packet length: %d (in)\n", packet_length);
            int result = picoquic_parse_header_and_decrypt(
                quic,                // quic: Picoquic context
                bytes_in,            // bytes: Input bytes
                length,              // length: Length of input bytes
                packet_length,       // packet_length: Total packet length
                NULL,                // addr_from
                current_time,        // current_time: Current time
                decrypted_data,      // decrypted_data: picoquic_stream_data_node_t to fill
                &ph,                 // ph: picoquic_packet_header to fill
                &cnx,                // pcnx:
                &consumed,           // consumed: Amount of consumed bytes?
                &new_context_created // new_context_created: 0 if there isn't a new context created?
            );
            bytes_out = decrypted_data->data; // Fixme: Set pointer for debug, should append for final test
            printf("[DEBUG] Result: %d (returned)\n", result);

            // Print statements for debug
            printf("[DEBUG] Packet header: (out)\n");
            char *ptype = NULL;
            ptype_to_string(ph.ptype, &ptype);
            printf("\t  Type: %s\n", ptype);
            printf("\t  DCID: ");
            print_byte_array(ph.dest_cnx_id.id, ph.dest_cnx_id.id_len);
            printf("\t  SCID: ");
            print_byte_array(ph.srce_cnx_id.id, ph.srce_cnx_id.id_len);
            printf("\t  Key phase: %d\n", ph.key_phase);
            printf("\t  Spin bit: %d\n", ph.spin);
            printf("\t  Packet number: %d\n", ph.pn);
            printf("\t  Packet number offset: %d\n", ph.pn_offset);
            printf("\t  Payload offset: %d\n", ph.offset);
            printf("\t  Payload length: %d\n", ph.payload_length);

            if (cnx != NULL)
            {
                printf("[DEBUG] Connection: (out)\n");
                printf("\t  Spin bit (enc): %d\n", cnx->key_phase_enc);
                printf("\t  Spin bit (dec): %d\n", cnx->key_phase_dec);
                printf("\t  Local CID: ");
                print_byte_array(cnx->path[0]->p_local_cnxid->cnx_id.id, cnx->path[0]->p_local_cnxid->cnx_id.id_len);
            }
            else
            {
                printf("[ERROR] Connection: NULL (out)\n");
            }

            printf("[DEBUG] Consumed: %d (out)\n", consumed);
            printf("[DEBUG] New context created: %s (out)\n", new_context_created ? "True" : "False");
            printf("[DEBUG] Bytes out: ");
            // print_byte_array(bytes_out, buffer_size);
            print_byte_array(bytes_out, msg_len);
        }
        else
        {
            printf("[ERROR] picoquic_stream_data_node_alloc for decrypted_data failed\n");
        }
    }
    else
    {
        printf("[ERROR] picoquic_create failed\n");
    }

    // Cleanup
    free(bytes_in);
    if (decrypted_data != NULL)
    {
        picoquic_stream_data_node_recycle(decrypted_data);
    }
    if (quic != NULL)
    {
        picoquic_free(quic);
    }

    // Exit program
    printf("\n[INFO] Function \"%s\" in \"%s\" is finished\n", __FUNCTION__, __FILE__);
    exit(exit_code);
}

/**
 * @brief Print a array's value and end with newline
 *
 * @param array Buffer to print
 * @param array_len Length of array to print
 * @return int Zero if success
 */
int print_byte_array(uint8_t *array, int array_len)
{
    for (int i = 0; i < array_len; i++)
    {
        printf("%02x", array[i]);
    }
    printf("\n");
    return 0;
}

/**
 * @brief Convert ptype to string presentation
 *
 * @param ptype ptype to convert
 * @param str output string representation
 * @return int Zero if success
 */
int ptype_to_string(picoquic_packet_type_enum ptype, char **str)
{
    switch (ptype)
    {
    case picoquic_packet_error:
        *str = "error";
        break;
    case picoquic_packet_version_negotiation:
        *str = "version_negotiation";
        break;
    case picoquic_packet_initial:
        *str = "initial";
        break;
    case picoquic_packet_retry:
        *str = "retry";
        break;
    case picoquic_packet_handshake:
        *str = "handshake";
        break;
    case picoquic_packet_0rtt_protected:
        *str = "0rtt_protected";
        break;
    case picoquic_packet_1rtt_protected:
        *str = "1rtt_protected";
        break;
    case picoquic_packet_type_max:
        *str = "type_max";
        break;

    default:
        *str = "unknown";
        break;
    }
}
