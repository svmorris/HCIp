#include <stdint.h>


#ifndef HCI_P_HEADER
#define HCI_P_HEADER


/*
 *              HCI packet types.
 *
 *  This section defines the structs used for storing HCI packet data.
 *  Each supported HCI packet has a corresponding struct. In these structs
 *  every byte of the original HCI packet is represented as structured c.
 */



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
    uint8_t  *rparams;
};



/*
 *              Spec defined constants.
 *
 * The following are constant values, taken directly from
 * the bluetooth core specification.
 */
typedef enum {
    HCI_COMMAND_PKT = 0x01,
    HCI_ACLDATA_PKT = 0x02,
    HCI_SCODATA_PKT = 0x03,
    HCI_EVENT_PKT   = 0x04,
    HCI_ISODATA_PKT = 0x05,
    HCI_VENDOR_PKT  = 0xff,
}hci_pkt_type;

typedef enum {
    HCI_UNKNOWN_COMMAND                 = 0x01,
    HCI_NO_CONNECTION                   = 0x02,
    HCI_HARDWARE_FAILURE                = 0x03,
    HCI_PAGE_TIMEOUT                    = 0x04,
    HCI_AUTHENTICATION_FAILURE          = 0x05,
    HCI_PIN_OR_KEY_MISSING              = 0x06,
    HCI_MEMORY_FULL                     = 0x07,
    HCI_CONNECTION_TIMEOUT              = 0x08,
    HCI_MAX_NUMBER_OF_CONNECTIONS       = 0x09,
    HCI_MAX_NUMBER_OF_SCO_CONNECTIONS   = 0x0a,
    HCI_ACL_CONNECTION_EXISTS           = 0x0b,
    HCI_COMMAND_DISALLOWED              = 0x0c,
    HCI_REJECTED_LIMITED_RESOURCES      = 0x0d,
    HCI_REJECTED_SECURITY               = 0x0e,
    HCI_REJECTED_PERSONAL               = 0x0f,
    HCI_HOST_TIMEOUT                    = 0x10,
    HCI_UNSUPPORTED_FEATURE             = 0x11,
    HCI_INVALID_PARAMETERS              = 0x12,
    HCI_OE_USER_ENDED_CONNECTION        = 0x13,
    HCI_OE_LOW_RESOURCES                = 0x14,
    HCI_OE_POWER_OFF                    = 0x15,
    HCI_CONNECTION_TERMINATED           = 0x16,
    HCI_REPEATED_ATTEMPTS               = 0x17,
    HCI_PAIRING_NOT_ALLOWED             = 0x18,
    HCI_UNKNOWN_LMP_PDU                 = 0x19,
    HCI_UNSUPPORTED_REMOTE_FEATURE      = 0x1a,
    HCI_SCO_OFFSET_REJECTED             = 0x1b,
    HCI_SCO_INTERVAL_REJECTED           = 0x1c,
    HCI_AIR_MODE_REJECTED               = 0x1d,
    HCI_INVALID_LMP_PARAMETERS          = 0x1e,
    HCI_UNSPECIFIED_ERROR               = 0x1f,
    HCI_UNSUPPORTED_LMP_PARAMETER_VALUE = 0x20,
    HCI_ROLE_CHANGE_NOT_ALLOWED         = 0x21,
    HCI_LMP_RESPONSE_TIMEOUT            = 0x22,
    HCI_LMP_ERROR_TRANSACTION_COLLISION = 0x23,
    HCI_LMP_PDU_NOT_ALLOWED             = 0x24,
    HCI_ENCRYPTION_MODE_NOT_ACCEPTED    = 0x25,
    HCI_UNIT_LINK_KEY_USED              = 0x26,
    HCI_QOS_NOT_SUPPORTED               = 0x27,
    HCI_INSTANT_PASSED                  = 0x28,
    HCI_PAIRING_NOT_SUPPORTED           = 0x29,
    HCI_TRANSACTION_COLLISION           = 0x2a,
    HCI_QOS_UNACCEPTABLE_PARAMETER      = 0x2c,
    HCI_QOS_REJECTED                    = 0x2d,
    HCI_CLASSIFICATION_NOT_SUPPORTED    = 0x2e,
    HCI_INSUFFICIENT_SECURITY           = 0x2f,
    HCI_PARAMETER_OUT_OF_RANGE          = 0x30,
    HCI_ROLE_SWITCH_PENDING             = 0x32,
    HCI_SLOT_VIOLATION                  = 0x34,
    HCI_ROLE_SWITCH_FAILED              = 0x35,
    HCI_EIR_TOO_LARGE                   = 0x36,
    HCI_SIMPLE_PAIRING_NOT_SUPPORTED    = 0x37,
    HCI_HOST_BUSY_PAIRING               = 0x38,
}hci_error_codes;

/*
 *              Table of supported values.
 *
 * All supported HCI packets are listed here. This used
 * to be a nice enum, but constraints have turned it to
 * a much uglier macro.
 */

/*
 * Macro:
 *   - name (enum)
 *   - prefix (for other variables)
 *   - value (from spec)
 *   - type (struct)
 */
#define EVENT_TYPES \
    X(EVT_CMD_STATUS,  evt_cmd_status, 0x0F, struct evt_cmd_status) \
    X(EVT_CMD_COMPLETE, evt_cmd_complete, 0x0E, struct evt_cmd_complete)



/*
 *              HCIp return types.
 *
 *  The following types are used for interfacing with the library,
 *  without the need to name the exact packet struct.
 */

typedef enum {
#define X(name, prefix, byte, type) name = byte,
    EVENT_TYPES
#undef X
} event_types_id;


typedef struct {
    event_types_id type;
    union {
#define X(name, prefix, byte, type) type name##_id;
        EVENT_TYPES
#undef X
    }u;
} event_pkt;



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
    PARSE_UNSUPPORTED = -1, // Correct, but unsupported features.
    PARSE_TRUNCATED   = -2, // In complete buffer.
    PARSE_INVALID     = -3, // Invalid HCI packets.
    PARSE_EINVAL      = -4, // User error.
    PARSE_ERROR       = -5, // Internal parser error.
}parse_error_codes;

/*
 *              HCIp Functions.
 */

int parse_event(uint8_t *event_bytecode, event_pkt *evt);
int encode_event(event_pkt *evt, uint8_t *event_bytecode);

#endif
