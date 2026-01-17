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

    uint8_t *params = &event_bytecode[offset];

    // Packet supplied bytes number
    uint8_t num_bytes = 0;

    for (size_t i = 0; i < field_desc_lens[event_code]; i++)
    {
        // TODO: I have no idea if this is correct.
        const struct field_desc *f = &field_descs[event_code] + i;

        // Pointer to where each value should live in the union
        uint8_t *dst = u + f->offset;

        switch (f->type)
        {
            case F_U8:
                *dst = params[offset];
                // NOTE: This assumes that num_bytes is always
                //       the first byte in a packet. This needs
                //       to be updated if that is proven wrong.
                //
                //       This will also seriously break if the
                //       first value of any packet is u16.
                if (i == 0)
                    num_bytes = params[offset];
                offset++;
                break;

            case F_U16:
                if (f->endian == ENDIAN_LE)
                {
                    uint16_t v = (uint16_t)params[offset] | ((uint16_t)params[offset+1] << 8);
                    memcpy(dst, &v, sizeof(uint16_t));
                }
                else
                {
                    *dst = params[offset];
                    *(dst+1) = params[offset+1];
                    offset += 2;
                }
                break;

            case F_BYTES:
                uint8_t *p = params + offset;
                *(uint8_t **)dst = p;
                offset += num_bytes - offset;

                break;


            default:
                // TODO: error handling
                break;
        }

    }

    return PARSE_OK;
}
