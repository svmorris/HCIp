#ifndef INTERNAL_PARSER_HEADER

#include <unistd.h>
#include <stdint.h>
#include <../include/hcip.h>

#define PARSER_HCI_MAX_DEPTH 0xa

typedef struct {
    const uint8_t *buf;
    size_t len;
    size_t offset;

    size_t depth;
    uint16_t path[PARSER_HCI_MAX_DEPTH]; // NOTE: I don't think any dispatches
                                         // will be made based on uint32_t values,
                                         // but if so, then this has to be updated.

    hci_pkt *returns;
}hci_parser_ctx;


// EVT dispatch table entries
typedef int (*evt_handler_t)(hci_parser_ctx *ctx);



#endif
