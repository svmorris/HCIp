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


/*
 * Error codes produced by the parser.
 *
 * If the parser returns a non-zero error code
 * look for these definitions and NOT the 
 * HCI error codes defined above.
 *
 * HCI error codes are reserved for errors
 * produced by the bluetooth controller.
 */
typedef enum {
    PARSE_OK          = 0, // Correctly parsed pkt.
    PARSE_UNSUPPORTED = 1, // Correct, but unsupported features.
    PARSE_TRUNCATED   = 2, // In complete buffer.
    PARSE_INVALID     = 3, // Invalid HCI packets.
    PARSE_EINVAL      = 4, // User error.
    PARSE_ERROR       = 5, // Internal parser error.
}parse_error_codes;

/* Descriptor table for evt_cmd_status */
extern const struct field_desc evt_cmd_status_desc[];
extern const size_t evt_cmd_status_desc_len;

extern const struct field_desc evt_cmd_complete_desc[];
extern const size_t evt_cmd_complete_desc_len;

extern const struct field_desc field_descs[];
extern const size_t field_desc_lens[];





/*
 * TODO: Need to change the definition of EVENT_TYPES to include
 *       this prefix value. type##_desc simply doesn't work
 * Definition:
 *   - name (enum)
 *   - prefix (for other variables)
 *   - byte (value)
 *   - type (struct)
 */
// #define EVENT_TYPES \
//     X(EVT_CMD_STATUS,  evt_cmd_status, 0x0F, struct evt_cmd_status) \
//     X(EVT_CMD_COMPLETE, evt_cmd_complete, 0x0E, struct evt_cmd_complete)


/*
 * Definition:
 *   - name (enum)
 *   - byte (value)
 *   - type (struct)
 */
#define EVENT_TYPES \
    X(EVT_CMD_STATUS,  0x0F, struct evt_cmd_status) \
    X(EVT_CMD_COMPLETE,0x0E, struct evt_cmd_complete)



// typedef enum {
//     EVT_CMD_STATUS   = 0x0F,
//     EVT_CMD_COMPLETE = 0x0E,
// } event_types_id;
typedef enum {
#define X(name, byte, type) name = byte,
    EVENT_TYPES
#undef X
} event_types_id;


// typedef struct {
//     event_types_id type;
//     union {
//         struct evt_cmd_status   EVT_CMD_STATUS_id;
//         struct evt_cmd_complete EVT_CMD_COMPLETE_id;
//     }u;
// } event_pkt;
typedef struct {
    event_types_id type;
    union {
#define X(name, byte, type) type name##_id;
        EVENT_TYPES
#undef X
    }u;
} event_pkt;




#endif
