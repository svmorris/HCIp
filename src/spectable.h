#ifndef SPECTABLE_H
#define SPECTABLE_H

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>


enum endianness {
    ENDIAN_NONE = 0,
    ENDIAN_LE   = 1,
    ENDIAN_BE   = 2,
};

enum field_type {
    F_U8,
    F_U16,
    F_BYTES,
};


struct field_desc {
    enum field_type type;
    size_t offset;     // offsetof(struct, member)
    size_t size;       // for F_BYTES or fixed arrays
    enum endianness endian;
};


struct evt_cmd_status {
    uint8_t  num_bytes;
    uint8_t  status;
    uint8_t  num_hci_commands;
    uint16_t opcode;
};

/* Descriptor table for evt_cmd_status */
extern const struct field_desc evt_cmd_status_desc[];
extern const size_t evt_cmd_status_desc_len;

#endif
