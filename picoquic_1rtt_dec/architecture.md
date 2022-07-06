# Picoquic 1-RTT decryption architecture

## Todo

- [ ] Create context 
- [ ] Set Connection IDs
- [ ] Set Keys
- [ ] Start measurements
- [ ] Send 1-RTT trafic through context
- [ ] Stop measurements
- [ ] Check correctness of output

## Intresting functions

1. `picoquic_incoming_packet` @ [packet.c:2454](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2454): "The incoming API allows a process to submit a packet through a QUIC context"
1. `picoquic_incoming_packet_ex` @ [packet.c:2406](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2406)
1. `picoquic_incoming_segment` @ [packet.c:2119](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L2119): "Processing of the packet that was just received from the network"
1. `picoquic_parse_header_and_decrypt` @ [packet.c:695](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L695)
    - `picoquic_parse_packet_header` @ [packet.c:357](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L357)
        - `picoquic_parse_short_packet_header`
    - `picoquic_remove_header_protection` @ [packet.c:445](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L445)
    - `picoquic_remove_packet_protection` @ [packet.c:543](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L543)
        - `picoquic_aead_decrypt_generic` @ [tls_api.c:247https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/tls_api.c): Calls `ptls_aead_decrypt`
1. `picoquic_incoming_1rtt` @ [packet.c:1956](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/packet.c#L1956): Parse frames?
    - `picoquic_decode_frames` @ [frames.c:4968](https://github.com/private-octopus/picoquic/tree/28b313c1ee483bfae784d33593d1e56a32701cc4/picoquic/frames.c#L4968)
