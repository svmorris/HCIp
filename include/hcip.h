// SPDX-License-Identifier: MIT


#ifndef HCI_PARSE_HEADER
#define HCI_PARSE_HEADER

#include <stdint.h>
#include <unistd.h>

// =========================== Spec Defined Constants ===========================
/*
 * Base level HCI packet types.
 */
typedef enum {
    HCI_COMMAND_PKT = 0x01,
    HCI_ACLDATA_PKT = 0x02,
    HCI_SCODATA_PKT = 0x03,
    HCI_EVENT_PKT   = 0x04,
    HCI_ISODATA_PKT = 0x05,
    HCI_VENDOR_PKT  = 0xff,
}hci_pkt_type;


/*
 * HCI Event packet types.
 */
typedef enum {
    EVT_INQUIRY_COMPLETE                  = 0x01,
    EVT_INQUIRY_RESULT                    = 0x02,
    EVT_CONN_COMPLETE                     = 0x03,
    EVT_CONN_REQUEST                      = 0x04,
    EVT_DISCONN_COMPLETE                  = 0x05,
    EVT_AUTH_COMPLETE                     = 0x06,
    EVT_REMOTE_NAME_REQ_COMPLETE          = 0x07,
    EVT_ENCRYPT_CHANGE                    = 0x08,
    EVT_CHANGE_CONN_LINK_KEY_COMPLETE     = 0x09,
    EVT_MASTER_LINK_KEY_COMPLETE          = 0x0A,
    EVT_READ_REMOTE_FEATURES_COMPLETE     = 0x0B,
    EVT_READ_REMOTE_VERSION_COMPLETE      = 0x0C,
    EVT_QOS_SETUP_COMPLETE                = 0x0D,
    EVT_CMD_STATUS                        = 0x0F,
    EVT_CMD_COMPLETE                      = 0x0E,
    EVT_HARDWARE_ERROR                    = 0x10,
    EVT_FLUSH_OCCURRED                    = 0x11,
    EVT_ROLE_CHANGE                       = 0x12,
    EVT_NUM_COMP_PKTS                     = 0x13,
    EVT_MODE_CHANGE                       = 0x14,
    EVT_RETURN_LINK_KEYS                  = 0x15,
    EVT_PIN_CODE_REQ                      = 0x16,
    EVT_LINK_KEY_REQ                      = 0x17,
    EVT_LINK_KEY_NOTIFY                   = 0x18,
    EVT_LOOPBACK_COMMAND                  = 0x19,
    EVT_DATA_BUFFER_OVERFLOW              = 0x1A,
    EVT_MAX_SLOTS_CHANGE                  = 0x1B,
    EVT_READ_CLOCK_OFFSET_COMPLETE        = 0x1C,
    EVT_CONN_PTYPE_CHANGED                = 0x1D,
    EVT_QOS_VIOLATION                     = 0x1E,
    EVT_PSCAN_REP_MODE_CHANGE             = 0x20,
    EVT_FLOW_SPEC_COMPLETE                = 0x21,
    EVT_INQUIRY_RESULT_WITH_RSSI          = 0x22,
    EVT_READ_REMOTE_EXT_FEATURES_COMPLETE = 0x23,
    EVT_SYNC_CONN_COMPLETE                = 0x2C,
    EVT_SYNC_CONN_CHANGED                 = 0x2D,
    EVT_SNIFF_SUBRATING                   = 0x2E,
    EVT_EXTENDED_INQUIRY_RESULT           = 0x2F,
    EVT_ENCRYPTION_KEY_REFRESH_COMPLETE   = 0x30,
    EVT_IO_CAPABILITY_RESPONSE            = 0x32,
    EVT_USER_CONFIRM_REQUEST              = 0x33,
    EVT_USER_PASSKEY_REQUEST              = 0x34,
    EVT_REMOTE_OOB_DATA_REQUEST           = 0x35,
    EVT_SIMPLE_PAIRING_COMPLETE           = 0x36,
    EVT_LINK_SUPERVISION_TIMEOUT_CHANGED  = 0x38,
    EVT_ENHANCED_FLUSH_COMPLETE           = 0x39,
    EVT_USER_PASSKEY_NOTIFY               = 0x3B,
    EVT_KEYPRESS_NOTIFY                   = 0x3C,
    EVT_REMOTE_HOST_FEATURES_NOTIFY       = 0x3D,
    EVT_LE_META_EVENT                     = 0x3E,

    EVT_CHANNEL_SELECTED                  = 0x41,
    EVT_PHYSICAL_LINK_COMPLETE            = 0x40,
    EVT_DISCONNECT_PHYSICAL_LINK_COMPLETE = 0x42,
    EVT_PHYSICAL_LINK_LOSS_EARLY_WARNING  = 0x43,
    EVT_PHYSICAL_LINK_RECOVERY            = 0x44,
    EVT_LOGICAL_LINK_COMPLETE             = 0x45,
    EVT_DISCONNECT_LOGICAL_LINK_COMPLETE  = 0x46,
    EVT_FLOW_SPEC_MODIFY_COMPLETE         = 0x47,
    EVT_NUMBER_COMPLETED_BLOCKS           = 0x48,

    EVT_AMP_STATUS_CHANGE                 = 0x4D,
    EVT_TESTING                           = 0xFE,
    EVT_VENDOR                            = 0xFF,
}hci_event_type;

/*
 * BLE meta event types.
 * These are one layer below `EVT_LE_META_EVENT` (0x3E)
 */
typedef enum {
    EVT_LE_CONN_COMPLETE                      = 0x00,
    EVT_LE_ADVERTISING_REPORT                 = 0x02,
    EVT_LE_CONN_UPDATE_COMPLETE               = 0x03,
    EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE = 0x04,
    EVT_LE_LTK_REQUEST                        = 0x05,
}hci_le_meta_event_type;

/*
 * Universal HCI error codes.
 */
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


// =========================== Parser Return Definitions ===========================
/*
 * Command status event.
 * This event is emitted by the controller when it needs
 * to signal something back to the host asynchronously.
 */
struct evt_cmd_status {
    uint8_t num_bytes;          // number of bytes returned
    uint8_t status;             // command status
    uint8_t num_hci_commands;   // number of HCI commands remaining
    uint16_t opcode;            // opcode in response to
};


/*
 * Command complete event.
 * This event is emitted by the controller when a host
 * requested command is completed.
 */
struct evt_cmd_complete {
    uint8_t num_bytes;          // number of bytes returned
    uint8_t num_hci_commands;   // number of HCI commands remaining
    uint16_t opcode;            // opcode in response to
    uint8_t return_len;         // length of returned data
    const uint8_t *data;        // returned data
};


/*
 * HCI Event packet.
 * Event packets are emitted by the controller
 * to give data back to the host.
 */
typedef struct {
    hci_event_type type;
    union {
        struct evt_cmd_status   status_evt;
        struct evt_cmd_complete complete_evt;
    } u;
}hci_event_pkt;


/*
 * Base level HCI packet.
 * This is the base return type of the parser. All
 * HCI packets should be fitted within this struct.
 */
typedef struct {
    hci_pkt_type type;
    union {
        hci_event_pkt event_pkt;
    } u;
}hci_pkt;





// =========================== Exposed Functions ===========================


typedef enum {
    PARSE_OK = 0,      // Correctly parsed pkt.
    PARSE_UNSUPPORTED, // Correct, but unsupported features.
    PARSE_TRUNCATED,   // In complete buffer.
    PARSE_INVALID,     // Invalid HCI packets.
    PARSE_EINVAL,      // User error.
    PARSE_ERROR,       // Internal parser error.
}parse_error_codes;


int parse_hci(uint8_t *buffer, size_t buf_len, hci_pkt pkt);

#endif
