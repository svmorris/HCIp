#include <stdio.h>
#include "parser.h"
#include "events.h"

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
    // Not using offset and depth here to make
    // it very clear that this is the first byte.
    uint8_t pkt_byte = ctx->buf[0];
    ctx->path[0] = (uint16_t)pkt_byte;
    ctx->offset++;
    ctx->depth++;

    // Probably need to set the correct union type somewhere
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


/*
 * Events dispatch table.
 * There are a lot of events so they definitely need a dispatch
 * table. Event if only a couple of them are implemented
 * right now.
 */

static int handler_pkt_event(hci_parser_ctx *ctx)
{

    evt_handler_t evt_dispatch_table[256] = {
        /* default for all unspecified opcodes */
        [0 ... 255] = handler_evt_unknown,

        /* command-related */
        [EVT_CMD_STATUS]   = handler_evt_status,
        [EVT_CMD_COMPLETE] = handler_evt_command,

        /* explicitly handled-but-not-yet-implemented events */
        [EVT_INQUIRY_COMPLETE]                  = handler_evt_unimplemented,
        [EVT_INQUIRY_RESULT]                    = handler_evt_unimplemented,
        [EVT_CONN_COMPLETE]                     = handler_evt_unimplemented,
        [EVT_CONN_REQUEST]                      = handler_evt_unimplemented,
        [EVT_DISCONN_COMPLETE]                  = handler_evt_unimplemented,
        [EVT_AUTH_COMPLETE]                     = handler_evt_unimplemented,
        [EVT_REMOTE_NAME_REQ_COMPLETE]          = handler_evt_unimplemented,
        [EVT_ENCRYPT_CHANGE]                    = handler_evt_unimplemented,
        [EVT_CHANGE_CONN_LINK_KEY_COMPLETE]     = handler_evt_unimplemented,
        [EVT_MASTER_LINK_KEY_COMPLETE]          = handler_evt_unimplemented,
        [EVT_READ_REMOTE_FEATURES_COMPLETE]     = handler_evt_unimplemented,
        [EVT_READ_REMOTE_VERSION_COMPLETE]      = handler_evt_unimplemented,
        [EVT_QOS_SETUP_COMPLETE]                = handler_evt_unimplemented,
        [EVT_HARDWARE_ERROR]                    = handler_evt_unimplemented,
        [EVT_FLUSH_OCCURRED]                    = handler_evt_unimplemented,
        [EVT_ROLE_CHANGE]                       = handler_evt_unimplemented,
        [EVT_NUM_COMP_PKTS]                     = handler_evt_unimplemented,
        [EVT_MODE_CHANGE]                       = handler_evt_unimplemented,
        [EVT_RETURN_LINK_KEYS]                  = handler_evt_unimplemented,
        [EVT_PIN_CODE_REQ]                      = handler_evt_unimplemented,
        [EVT_LINK_KEY_REQ]                      = handler_evt_unimplemented,
        [EVT_LINK_KEY_NOTIFY]                   = handler_evt_unimplemented,
        [EVT_LOOPBACK_COMMAND]                  = handler_evt_unimplemented,
        [EVT_DATA_BUFFER_OVERFLOW]              = handler_evt_unimplemented,
        [EVT_MAX_SLOTS_CHANGE]                  = handler_evt_unimplemented,
        [EVT_READ_CLOCK_OFFSET_COMPLETE]        = handler_evt_unimplemented,
        [EVT_CONN_PTYPE_CHANGED]                = handler_evt_unimplemented,
        [EVT_QOS_VIOLATION]                     = handler_evt_unimplemented,
        [EVT_PSCAN_REP_MODE_CHANGE]             = handler_evt_unimplemented,
        [EVT_FLOW_SPEC_COMPLETE]                = handler_evt_unimplemented,
        [EVT_INQUIRY_RESULT_WITH_RSSI]          = handler_evt_unimplemented,
        [EVT_READ_REMOTE_EXT_FEATURES_COMPLETE] = handler_evt_unimplemented,
        [EVT_SYNC_CONN_COMPLETE]                = handler_evt_unimplemented,
        [EVT_SYNC_CONN_CHANGED]                 = handler_evt_unimplemented,
        [EVT_SNIFF_SUBRATING]                   = handler_evt_unimplemented,
        [EVT_EXTENDED_INQUIRY_RESULT]           = handler_evt_unimplemented,
        [EVT_ENCRYPTION_KEY_REFRESH_COMPLETE]   = handler_evt_unimplemented,
        [EVT_IO_CAPABILITY_RESPONSE]            = handler_evt_unimplemented,
        [EVT_USER_CONFIRM_REQUEST]              = handler_evt_unimplemented,
        [EVT_USER_PASSKEY_REQUEST]              = handler_evt_unimplemented,
        [EVT_REMOTE_OOB_DATA_REQUEST]           = handler_evt_unimplemented,
        [EVT_SIMPLE_PAIRING_COMPLETE]           = handler_evt_unimplemented,
        [EVT_LINK_SUPERVISION_TIMEOUT_CHANGED]  = handler_evt_unimplemented,
        [EVT_ENHANCED_FLUSH_COMPLETE]           = handler_evt_unimplemented,
        [EVT_USER_PASSKEY_NOTIFY]               = handler_evt_unimplemented,
        [EVT_KEYPRESS_NOTIFY]                   = handler_evt_unimplemented,
        [EVT_REMOTE_HOST_FEATURES_NOTIFY]       = handler_evt_unimplemented,
        [EVT_LE_META_EVENT]                     = handler_evt_unimplemented,

        /* AMP / physical link */
        [EVT_PHYSICAL_LINK_COMPLETE]            = handler_evt_unimplemented,
        [EVT_CHANNEL_SELECTED]                  = handler_evt_unimplemented,
        [EVT_DISCONNECT_PHYSICAL_LINK_COMPLETE] = handler_evt_unimplemented,
        [EVT_PHYSICAL_LINK_LOSS_EARLY_WARNING]  = handler_evt_unimplemented,
        [EVT_PHYSICAL_LINK_RECOVERY]            = handler_evt_unimplemented,
        [EVT_LOGICAL_LINK_COMPLETE]             = handler_evt_unimplemented,
        [EVT_DISCONNECT_LOGICAL_LINK_COMPLETE]  = handler_evt_unimplemented,
        [EVT_FLOW_SPEC_MODIFY_COMPLETE]         = handler_evt_unimplemented,
        [EVT_NUMBER_COMPLETED_BLOCKS]           = handler_evt_unimplemented,
        [EVT_AMP_STATUS_CHANGE]                 = handler_evt_unimplemented,

        /* vendor / testing */
        [EVT_TESTING]                           = handler_evt_unimplemented,
        [EVT_VENDOR]                            = handler_evt_unimplemented,
    };

    uint8_t evt_byte = ctx->buf[offset];
    ctx->path[ctx->depth] = evt_byte;
    ctx->offset++;
    ctx->depth++;

    return evt_dispatch_table[evt_code](ctx);
}

static int handler_pkt_command(hci_parser_ctx *ctx)
{
    puts("command pkt");
    return PARSE_OK;
}
