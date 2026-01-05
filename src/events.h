#ifndef PARSER_EVENTS_HEADER
#define PARSER_EVENTS_HEADER

struct hci_parser_ctx;   // forward declaration

int handler_evt_status(hci_parser_ctx *ctx);
int handler_evt_command_complete(hci_parser_ctx *ctx);
int handler_evt_unknown(hci_parser_ctx *ctx);
int handler_evt_unimplemented(hci_parser_ctx *ctx);
#endif
