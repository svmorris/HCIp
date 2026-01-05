#include "parser.h"
#include "events.h"


int handler_evt_status(hci_parser_ctx *ctx)
{

    hci_pkt *pkt = ctx->returns;

    pkt->type = HCI_EVENT_PKT;
    pkt->u.event_pkt.type = EVT_CMD_STATUS;

    // At least 5 bytes needed for a correct EVT.
    if ((ctx->len - ctx->offset) < 5)
        return PARSE_INVALID;


    struct evt_cmd_status *evt = &pkt->u.event_pkt.u.status_evt;

    evt->num_bytes = ctx->buf[ctx->offset++];

    // This number is well defined in the spec.
    if (evt->num_bytes != 4)
        return PARSE_INVALID;

    evt->status           = ctx->buf[ctx->offset++];
    evt->num_hci_commands = ctx->buf[ctx->offset++];

    evt->opcode = ctx->buf[ctx->offset] | (ctx->buf[ctx->offset+1] << 8);
    ctx->offset += 2;

    return PARSE_OK;
}


int handler_evt_command_complete(hci_parser_ctx *ctx)
{

    hci_pkt *pkt = ctx->returns;

    pkt->type = HCI_EVENT_PKT;
    pkt->u.event_pkt.type = EVT_CMD_COMPLETE;

    // Min 5 bytes
    if ((ctx->len - ctx->offset) < 4)
        return PARSE_INVALID;

    struct evt_cmd_complete *evt = &pkt->u.event_pkt.u.complete_evt;

    evt->num_bytes = ctx->buf[ctx->offset++];
    evt->num_hci_commands = ctx->buf[ctx->offset++];
    evt->opcode = ctx->buf[ctx->offset] | (ctx->buf[ctx->offset+1] << 8);
    ctx->offset += 2;
    evt->return_len = evt->num_bytes - 3;

    // Num bytes is the source of truth. If we get less than its not the
    // whole packet.
    if ((ctx->len - ctx->offset) < evt->num_bytes)
        return PARSE_TRUNCATED;

    evt->data = &ctx->buf[ctx->offset];
    ctx->offset += evt->return_len;
    return PARSE_OK;
}




int handler_evt_unimplemented(hci_parser_ctx *ctx)
{
    // just temp to suppress warnings
    // yes i know its disgusting
    ctx->len = 0;
    return PARSE_UNSUPPORTED;
}



int handler_evt_unknown(hci_parser_ctx *ctx)
{
    // just temp to suppress warnings
    // yes i know its disgusting
    ctx->len = 0;
    return PARSE_INVALID;
}
