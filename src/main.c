#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "spectable.h"

int parse_event(uint8_t *example, struct evt_cmd_status *evt);
int encode_event(uint8_t *buffer, struct evt_cmd_status *evt);

int main()
{
    uint8_t example[] = {0x04, 0x0f, 0x04, 0x01, 0x01, 0x0c, 0x02};
    struct evt_cmd_status evt = {0};


    if (example[0] == 0x04)
        parse_event(example, &evt);

    putchar('\n');
    putchar('\n');

    uint8_t *buffer = malloc(256);
    memset(buffer, 0, 256);
    encode_event(buffer, &evt);
}




int parse_event(uint8_t *example, struct evt_cmd_status *evt)
{
    printf("Parsing example packet: ");
    for (size_t i = 0; i < 7; i++)
    {
        printf("%02x ", example[i]);
    }
    putchar('\n');


    if (example[1] != 0x0f)
        return 1; //unimplemented

    // get a pointer to the number of parameters
    uint8_t *params = &example[2];

    // we are counting offset from the parameters not from the start of the packet
    size_t offset = 0;
    /* puts("a"); */


    // using the spec defined length not the one in the bytes
    // Loop over each byte
    for (size_t i = 0; i < evt_cmd_status_desc_len; i++)
    {

        // Using f to get the field description row for each one
        const struct field_desc *f = &evt_cmd_status_desc[i];

        // Getting a pointer to the same offset but in the evt struct
        uint8_t *dst = (uint8_t *)evt + f->offset;

        // switch on the field type (as defined in the description table)
        switch (f->type)
        {
            // uint8_t bytes are simple
            case F_U8:
                *dst = params[offset];
                offset += 1;
                break;

            // For uint16 bytes we need to switch on whether they are little or big endien
            // for now its only handling the LE types
            case F_U16:
                if (f->endian == ENDIAN_LE)
                {
                    uint16_t v = (uint16_t)params[offset] | ((uint16_t)params[offset+1] << 8);
                    memcpy(dst, &v, sizeof(uint16_t));
                }
                else{}
                offset += 2;
                break;

            // Don't yet have a description for bytes
            // will most likely just be a pointer to the original buffer
            // with a length field.
            case F_BYTES:
                // not implemented
                break;

            default:
                break;
        }
    }

    printf("EVT_CMD_STATUS parsed:\n");
    printf("  num_bytes        = %u\n", evt->num_bytes);
    printf("  status           = 0x%02x\n", evt->status);
    printf("  num_hci_commands = %u\n", evt->num_hci_commands);
    printf("  opcode           = 0x%04x\n", evt->opcode);

    return 0;
}


int encode_event(uint8_t *buffer, struct evt_cmd_status *evt)
{
    printf("EVT_CMD_STATUS to be encoded:\n");
    printf("  num_bytes        = %u\n", evt->num_bytes);
    printf("  status           = 0x%02x\n", evt->status);
    printf("  num_hci_commands = %u\n", evt->num_hci_commands);
    printf("  opcode           = 0x%04x\n", evt->opcode);

    buffer[0] = 0x04;
    buffer[1] = 0x0f;

    int offset = 2;

    for (size_t i = 0; i < evt_cmd_status_desc_len; i++)
    {
        const struct field_desc *f = &evt_cmd_status_desc[i];

        // the destination is now the buffer
        uint8_t *dst = buffer + offset;


        switch (f->type)
        {
            case F_U8:
                *dst = ((uint8_t *)evt)[f->offset];
                offset += 1;
                break;

            case F_U16:
                if (f->endian == ENDIAN_LE)
                {
                    uint16_t v = ((uint8_t *)evt)[f->offset] | (((uint8_t *)evt)[f->offset+1] << 8);
                    memcpy(dst, &v, sizeof(uint16_t));
                }
                else{}
                offset += 2;
                break;
            default:
                break;
        }

    }

    printf("Encoded packet:");
    for (size_t i = 0; i < 7; i++)
    {
        printf("%02x ", buffer[i]);
    }
    putchar('\n');

    return 0;
}

