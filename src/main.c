#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "spectable.h"

int parse_event(uint8_t *example, struct evt_cmd_status *evt);
int encode_event(uint8_t *buffer, struct evt_cmd_status *evt);
int parse_event_complete(uint8_t *example, struct evt_cmd_complete *evt);
int encode_event_complete(uint8_t *buffer, struct evt_cmd_complete *evt);

int main()
{
    uint8_t example[] = {0x04, 0x0f, 0x04, 0x01, 0x01, 0x0c, 0x02};
    struct evt_cmd_status evt = {0};


    if (example[0] == 0x04)
        parse_event(example, &evt);

    putchar('\n');
    putchar('\n');

    /*
     * Note: We just assume that the client will be able to
     * provide a buffer large enough for the returned data.
     *
     * This is obviously not a great solution since the client
     * has no idea how long it will be. A problem for future me
     * to fix.
     */
    uint8_t *buffer = malloc(256);
    memset(buffer, 0, 256);
    encode_event(buffer, &evt);


    putchar('\n');
    putchar('\n');

    uint8_t example1[] = { 0x04, 0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x0d, 0x85, 0x30, 0x0d, 0x02, 0x00, 0x85, 0x30 };
    struct evt_cmd_complete evtc = {0};

    parse_event_complete(example1, &evtc);

    putchar('\n');
    putchar('\n');


    memset(buffer, 0, 256);
    encode_event_complete(buffer, &evtc);
}


int parse_event_complete(uint8_t *example, struct evt_cmd_complete *evt)
{
    printf("Parsing example complete packet: ");
    for (size_t i = 0; i < 15; i++)
    {
        printf("%02x ", example[i]);
    }
    putchar('\n');


    if (example[1] != 0x0e)
        return 1; //unimplemented

    // get a pointer to the number of parameters
    uint8_t *params = &example[2];

    // we are counting offset from the parameters not from the start of the packet
    size_t offset = 0;
    /* puts("a"); */


    // using the spec defined length not the one in the bytes
    // Loop over each byte
    for (size_t i = 0; i < evt_cmd_complete_desc_len; i++)
    {

        // Using f to get the field description row for each one
        const struct field_desc *f = &evt_cmd_complete_desc[i];

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
                uint8_t *p = params + offset;
                *(uint8_t **)dst = p;
                offset += (evt->num_bytes - offset);
                break;

            default:
                break;
        }
    }

    printf("EVT_CMD_COMPLETE parsed:\n");
    printf("  num_bytes        = %02x\n", evt->num_bytes);
    printf("  num_hci_commands = %02x\n", evt->num_hci_commands);
    printf("  opcode           = 0x%04x\n", evt->opcode);
    printf("  status           = 0x%02x\n", evt->status);
    printf("  Packet bytes: ");
    uint8_t *p1 = *(uint8_t **)evt->rparams;
    // The number 4 here represents the bytes in the struct
    // other than num bytes. It is the best way we have to determine
    // the length of the array. When this gets generalised, I will
    // need to find a more generalisable method.
    for (size_t i = 0; i < evt->num_bytes-4; i++)
        printf("%02x ", p1[i]);
    putchar('\n');

    return 0;
}


int parse_event(uint8_t *example, struct evt_cmd_status *evt)
{
    printf("Parsing example status packet: ");
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

int encode_event_complete(uint8_t *buffer, struct evt_cmd_complete *evt)
{
    printf("EVT_CMD_COMPLETE to be encoded:\n");
    printf("  num_bytes        = %u\n", evt->num_bytes);
    printf("  status           = 0x%02x\n", evt->status);
    printf("  num_hci_commands = %u\n", evt->num_hci_commands);
    printf("  opcode           = 0x%04x\n", evt->opcode);

    buffer[0] = 0x04;
    buffer[1] = 0x0e;

    int offset = 2;

    for (size_t i = 0; i < evt_cmd_complete_desc_len; i++)
    {
        const struct field_desc *f = &evt_cmd_complete_desc[i];

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
            case F_BYTES:
                // + 3 might seem like quite an arbitrary number here. It isn't, but getting
                // to it is not clear.
                // offset+3 signifies the bytes consumed after num_bytes, this is because
                // the loop started with the offset of 2 and num_bytes was the first value.
                printf("Size calculated from num_bytes: %x\n", evt->num_bytes-offset+3);

                // It might also not seem like the cleanest solution to rely on adding
                // or removing values from num bytes as the best way to store the length
                // of this buffer. You are right, but I'm not yet sure if its actually bad.
                //
                // I'm not sure if it would be worse if instead I would have to make
                // custom fields within the struct for buffer lengths. As long as each
                // packet only has one type of array and each packet always starts with
                // num_bytes, it *should* work.
                //
                // Also this is just the initial implementation that will 100% be re-written.
                memcpy(dst, *(uint8_t **)evt->rparams, (evt->num_bytes-offset+3));
                offset = offset + (evt->num_bytes-offset+3);
                break;
            default:
                break;
        }

    }

    printf("Encoded packet:");
    for (size_t i = 0; i < evt->num_bytes+3; i++)
    {
        printf("%02x ", buffer[i]);
    }
    putchar('\n');

    return 0;
}
