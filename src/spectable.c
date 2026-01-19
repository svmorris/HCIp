#include "parser.h"

/*
 * NOTE:
 *  If any packet's first value is not `num_bytes` as F_U8,
 *  then the parser has to be updated.
 *
 * NOTE_1:
 *  Non-LE F_U16 values are not implemented yet.
 */

const struct field_desc evt_cmd_status_desc[] = {
    { F_U8,  offsetof(struct evt_cmd_status, num_bytes),        0, ENDIAN_NONE },
    { F_U8,  offsetof(struct evt_cmd_status, status),           0, ENDIAN_NONE },
    { F_U8,  offsetof(struct evt_cmd_status, num_hci_commands), 0, ENDIAN_NONE },
    { F_U16, offsetof(struct evt_cmd_status, opcode),           0, ENDIAN_LE   },
};

const size_t evt_cmd_status_desc_len =
    sizeof(evt_cmd_status_desc) / sizeof(evt_cmd_status_desc[0]);


/*  uint8_t buffer[] = { 0x04, 0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x0d, 0x85, 0x30, 0x0d, 0x02, 0x00, 0x85, 0x30 }; */
const struct field_desc evt_cmd_complete_desc[] = {
    { F_U8,    offsetof(struct evt_cmd_complete, num_bytes),        0, ENDIAN_NONE },
    { F_U8,    offsetof(struct evt_cmd_complete, num_hci_commands), 0, ENDIAN_NONE },
    { F_U16,   offsetof(struct evt_cmd_complete, opcode),           0, ENDIAN_LE   },
    { F_U8,    offsetof(struct evt_cmd_complete, status),           0, ENDIAN_NONE },
    { F_BYTES, offsetof(struct evt_cmd_complete, rparams),          0, ENDIAN_BE   },
};
const size_t evt_cmd_complete_desc_len =
    sizeof(evt_cmd_complete_desc) / sizeof(evt_cmd_complete_desc[0]);



const size_t field_desc_lens[0xFF] = {
#define X(name, prefix, byte, type) [byte] = prefix##_desc_len,
    EVENT_TYPES
#undef X
};


const struct field_desc *field_descs[0xFF] = {
#define X(name, prefix, byte, type) [byte] = prefix##_desc,
    EVENT_TYPES
#undef X
};
