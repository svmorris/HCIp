#include "parser.h"
#include <string.h>

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
                return PARSE_ERROR;
                break;
        }
    }
    return PARSE_OK;
}

int encode_event(event_pkt *evt, uint8_t* event_bytecode)
{
    event_bytecode[0] = 0x04;
    event_bytecode[1] = (uint8_t)evt->type;
    int offset = 2;

    void *u = (uint8_t *)evt+offsetof(event_pkt, u);
    uint8_t num_bytes = 0;


    for (size_t i = 0; i < field_desc_lens[(uint8_t)evt->type]; i++)
    {
        const struct field_desc *f = field_descs[(uint8_t)evt->type] + i;

        uint8_t *src = u + f->offset;

        switch (f->type)
        {
            case F_U8:
                event_bytecode[offset] = *src;
                offset++;
                if (i == 0)
                    num_bytes = (uint8_t)*src;
                break;
            case F_U16:
                if(f->endian == ENDIAN_LE)
                {
                    uint16_t v = (uint16_t)src[0] | ((uint16_t)src[1] << 8);
                    memcpy(&event_bytecode[offset], &v, sizeof(uint16_t));
                }
                else
                {
                    memcpy(&event_bytecode[offset], src, sizeof(uint16_t));
                }
                offset += 2;
                break;

            case F_BYTES:
                // +3 is not an arbitrary number. It is the current position
                // of the byte string after removing the packet type, event type
                // and num_bytes value.
                memcpy(&event_bytecode[offset], *(uint8_t **)src, num_bytes-offset+3);
                offset = offset + (num_bytes-offset+3);
                break;

            default:
                return PARSE_ERROR;
                break;

        }
    }

    return offset;
}
