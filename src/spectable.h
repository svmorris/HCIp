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

/*
 * The bluetooth spec doesn't explicitly guarantee that
 * the first byte of return parameters will be the status
 * byte. In practice, this seems to be always the case.
 *
 * For now, this parser will parse out the status as its
 * own field, since I believe its a better choice than
 * leaving the user to deal with it. If I come across
 * any packet that does not respect this rule then
 * the inclusion of the status byte should be re-evaluated.
 *
 * Alternatively there might be a need for command_complete
 * specific actions, but I will try to keep the parser
 * general with no specific checks for as long as it makes sense.
 */
struct evt_cmd_complete {
    uint8_t  num_bytes;
    uint8_t  num_hci_commands;
    uint16_t opcode;
    uint8_t  status;
    uint8_t  rparams[];
};

/* Descriptor table for evt_cmd_status */
extern const struct field_desc evt_cmd_status_desc[];
extern const size_t evt_cmd_status_desc_len;

extern const struct field_desc evt_cmd_complete_desc[];
extern const size_t evt_cmd_complete_desc_len;

#endif
