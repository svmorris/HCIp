#include <stdio.h>
#include "parser.h"

static int dispatch_pkt_type(hci_parser_ctx *ctx);
static int handler_pkt_event(hci_parser_ctx *ctx);
static int handler_pkt_command(hci_parser_ctx *ctx);



int parse_hci(uint8_t *buffer, size_t buf_len, hci_pkt pkt)
{
    if (buffer == NULL || buf_len < 3)
        return PARSE_EINVAL;

    hci_parser_ctx ctx = {
        .buf = buffer,
        .len = buf_len,
        .offset = 0,
        .depth = 0,
        .path = {},
        .returns = &pkt
    };

    return dispatch_pkt_type(&ctx);
}



static int dispatch_pkt_type(hci_parser_ctx *ctx)
{
    uint8_t pkt_byte = ctx->buf[0];
    ctx->offset++;
    ctx->depth++;
    ctx->path[0] = (uint16_t)pkt_byte;

    switch (pkt_byte)
    {
        case HCI_COMMAND_PKT:
              return handler_pkt_command(ctx);
              break;
        case HCI_ACLDATA_PKT:
              return PARSE_UNSUPPORTED;
              break;
        case HCI_SCODATA_PKT:
              return PARSE_UNSUPPORTED;
              break;
        case HCI_EVENT_PKT:
              return handler_pkt_event(ctx);
              break;
        case HCI_ISODATA_PKT:
              return PARSE_UNSUPPORTED;
              break;
        case HCI_VENDOR_PKT:
              return PARSE_UNSUPPORTED;
              break;
        default:
              return PARSE_INVALID;
    }
}


static int handler_pkt_event(hci_parser_ctx *ctx)
{
    puts("event pkt");
    return PARSE_OK;
}

static int handler_pkt_command(hci_parser_ctx *ctx)
{
    puts("command pkt");
    return PARSE_OK;
}
