#include <stdint.h>


// =========================== Parser Return Definitions ===========================


/*
 * Enums used for determining the packet layers
 */
typedef enum {
    EVT_CMD_COMPLETE,
    EVT_CMD_STATUS,
}hci_event_type;

typedef enum {
    HCI_EVENT_PKT,
    HCI_COMMAND_PKT,
}hci_pkt_type;


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


