#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "spectable.h"

int parse_event(uint8_t *event_bytecode, event_pkt *evt);

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
    putchar('\n');


    uint8_t example1[] = { 0x04, 0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x0d, 0x85, 0x30, 0x0d, 0x02, 0x00, 0x85, 0x30 };
    event_pkt evt1 = {0};

    printf("Parsing example event: ");
    for (int i = 0; i < (int)sizeof(example1); i++)
        printf("%02x ", example1[i]);
    putchar('\n');

    parse_event(example1, &evt1);


    if (evt1.type == EVT_CMD_COMPLETE)
    {
        struct evt_cmd_complete complete = evt1.u.EVT_CMD_COMPLETE_id;

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


}


int parse_event(uint8_t *event_bytecode, event_pkt *evt)
{
    int offset = 0;
    if (event_bytecode[offset] != 0x04)
        return PARSE_UNSUPPORTED;
    offset++;

    uint8_t event_code = event_bytecode[offset];
    evt->type = event_code;
    offset++;

    // Gets a pointer to U without naming the actual event struct
    void *u = (uint8_t *)evt + offsetof(event_pkt, u);
    memset(u, 0, sizeof(evt->u));

    // Packet supplied bytes number
    uint8_t num_bytes = 0;

    for (size_t i = 0; i < field_desc_lens[event_code]; i++)
    {
        // TODO: I have no idea if this is correct.
        const struct field_desc *f = field_descs[event_code] + i;

        // Pointer to where each value should live in the union
        uint8_t *dst = u + f->offset;

        switch (f->type)
        {
            case F_U8:
                *dst = event_bytecode[offset];
                // NOTE: This assumes that num_bytes is always
                //       the first byte in a packet. This needs
                //       to be updated if that is proven wrong.
                //
                //       This will also seriously break if the
                //       first value of any packet is u16.
                if (i == 0)
                    num_bytes = event_bytecode[offset];
                offset++;
                break;

            case F_U16:
                if (f->endian == ENDIAN_LE)
                {
                    uint16_t v = (uint16_t)event_bytecode[offset] | ((uint16_t)event_bytecode[offset+1] << 8);
                    memcpy(dst, &v, sizeof(uint16_t));
                }
                else
                {
                    *dst = event_bytecode[offset];
                    *(dst+1) = event_bytecode[offset+1];
                }
                offset += 2;
                break;

            case F_BYTES:
                uint8_t *p = event_bytecode + offset;
                *(uint8_t **)dst = p;
                offset += num_bytes - offset;

            default:
                // TODO: error handling
                break;
        }
    }
    return PARSE_OK;
}
