#ifndef PARSER_EVENTS_HEADER
#include "parser.h"

int handler_evt_status(hci_parser_ctx *ctx);
int hanlder_evt_command(hci_parser_ctx *ctx);
int handler_evt_unknown(hci_parser_ctx *ctx);
int handler_evt_unimplemented(hci_parser_ctx *ctx);
#endif
