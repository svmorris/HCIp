#ifndef HCIP_PARSER_H
#define HCIP_PARSER_H

#include "../include/HCIp.h"

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

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


extern const struct field_desc evt_cmd_status_desc[];
extern const size_t evt_cmd_status_desc_len;

extern const struct field_desc evt_cmd_complete_desc[];
extern const size_t evt_cmd_complete_desc_len;

extern const struct field_desc *field_descs[];
extern const size_t field_desc_lens[];




#endif
