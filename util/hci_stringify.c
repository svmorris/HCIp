#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hci_stringify.h"
#include "../include/hcip.h"


static const char *const hci_pkt_type_name[] = {
    [HCI_COMMAND_PKT] = "[Command]",
    [HCI_ACLDATA_PKT] = "[ACL Data]",
    [HCI_SCODATA_PKT] = "[SCO Data]",
    [HCI_EVENT_PKT]   = "[Event]",
    [HCI_ISODATA_PKT] = "[ISO Data]",
    [HCI_VENDOR_PKT]  = "[Vendor]",
};


static const char *const hci_global_kind_name[] = {
    [HKIND_UNKNOWN]              = "[Unknown packet]",
    [HKIND_EVT_COMMAND_STATUS]   = "[Command Status]",
    [HKIND_EVT_COMMAND_COMPLETE] = "[Command Complete]"
};


static int write_buf(hci_strbuf *out, char *text);
const char *hci_pkt_type_to_string(uint8_t type);
const char *hci_global_kind_to_string(uint8_t type);



/*
 * Main entry point.
 */
int hci_stringify(hci_pkt *pkt, hci_strbuf *out)
{
    int ret = 0;

    ret = write_buf(out, (char*)hci_pkt_type_to_string(pkt->type));
    if (ret != S_OK)
        return ret;

    ret = write_buf(out, (char*)hci_global_kind_to_string(pkt->kind));
    if (ret != S_OK)
        return ret;

    return S_OK;
}



static int write_buf(hci_strbuf *out, char *text)
{

    size_t write_len = strlen(text)+1;
    if ((write_len + out->len) > out->cap)
        return S_OUTOFBOUNDS;

    if (snprintf(out->buf, write_len, "%s", text) < 0)
        return S_INTERNALERR;

    out->len += write_len;
    return S_OK;
}



const char *hci_global_kind_to_string(uint8_t type)
{
    if (hci_global_kind_name[type])
        return hci_global_kind_name[type];

    return "[Unknown Kind]";
}

const char *hci_pkt_type_to_string(uint8_t type)
{
    if (hci_pkt_type_name[type])
        return hci_pkt_type_name[type];

    return "[Unknown Packet Type]";
}
