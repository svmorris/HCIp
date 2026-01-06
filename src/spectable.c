#include "spectable.h"


const struct field_desc evt_cmd_status_desc[] = {
    { F_U8,  offsetof(struct evt_cmd_status, num_bytes),        0, ENDIAN_NONE },
    { F_U8,  offsetof(struct evt_cmd_status, status),           0, ENDIAN_NONE },
    { F_U8,  offsetof(struct evt_cmd_status, num_hci_commands), 0, ENDIAN_NONE },
    { F_U16, offsetof(struct evt_cmd_status, opcode),           0, ENDIAN_LE   },
};

const size_t evt_cmd_status_desc_len =
    sizeof(evt_cmd_status_desc) / sizeof(evt_cmd_status_desc[0]);
