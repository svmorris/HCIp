#include "../include/HCIp.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    uint8_t example[] = { 0x04, 0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x0d, 0x85, 0x30, 0x0d, 0x02, 0x00, 0x85, 0x30 };
    event_pkt evt = {0};

    printf("Parsing example event: ");
    for (int i = 0; i < (int)sizeof(example); i++)
        printf("%02x ", example[i]);
    putchar('\n');

    parse_event(example, &evt);


    if (evt.type == EVT_CMD_COMPLETE)
    {
        struct evt_cmd_complete complete = evt.u.EVT_CMD_COMPLETE_id;

        printf("EVT_CMD_COMPLETE parsed:\n");
        printf("  num_bytes        = %02x\n", complete.num_bytes);
        printf("  num_hci_commands = %02x\n", complete.num_hci_commands);
        printf("  opcode           = 0x%04x\n", complete.opcode);
        printf("  status           = 0x%02x\n", complete.status);
        printf("  Packet bytes: ");

        // The number 4 here represents the bytes in the struct
        // other than num bytes. It is the best way we have to determine
        // the length of the array. When this gets generalised, I will
        // need to find a more generalisable method.
        for (int i = 0; i < complete.num_bytes-4; i++)
        {
            printf("%02x ", complete.rparams[i]);
        }
        putchar('\n');
    }
    putchar('\n');

    printf("Re-encoding packet: ");
    uint8_t *buf = malloc(0xff);
    int n = encode_event(&evt, buf);
    if (n < 0)
    {
        printf("\nParsing error: %d\n", n);
        return n;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%02x ", buf[i]);
    }
    putchar('\n');


    for (int i = 0; i < n; i++)
    {
        if (buf[i] != example[i])
        {
            return -1;
        }
    }
    return 0;
}
