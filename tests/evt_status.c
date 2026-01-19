#include "../src/parser.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    uint8_t example[] = {0x04, 0x0f, 0x04, 0x01, 0x01, 0x0c, 0x02};
    event_pkt evt = {0};

    printf("Parsing example event: ");
    for (int i = 0; i < (int)sizeof(example); i++)
        printf("%02x ", example[i]);
    putchar('\n');

    parse_event(example, &evt);

    if (evt.type == EVT_CMD_STATUS)
    {
        struct evt_cmd_status status = evt.u.EVT_CMD_STATUS_id;

        printf("EVT_CMD_STATUS parsed:\n");
        printf("  num_bytes        = %u\n", status.num_bytes);
        printf("  status           = 0x%02x\n", status.status);
        printf("  num_hci_commands = %u\n", status.num_hci_commands);
        printf("  opcode           = 0x%04x\n", status.opcode);
    }

    putchar('\n');


    printf("Re-encoding packet: ");
    uint8_t *sbuf = malloc(0xff);
    int n = encode_event(&evt, sbuf);
    if (n < 0)
    {
        printf("\nParsing error: %d\n", n);
        return n;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%02x ", sbuf[i]);
    }
    putchar('\n');


    for (int i = 0; i < n; i++)
    {
        if (sbuf[i] != example[i])
        {
            return -1;
        }
    }

    return 0;
}
