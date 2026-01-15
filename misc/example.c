#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Example event payloads */
struct evt_cmd_status {
    uint8_t status;
};

struct evt_cmd_complete {
    uint16_t opcode;
};

/* X-macro list */
#define EVENT_TYPES \
    X(EVT_CMD_STATUS,   0x0F, struct evt_cmd_status) \
    X(EVT_CMD_COMPLETE, 0x0E, struct evt_cmd_complete)


/* Enum generated from X-macro */
/* typedef enum { */
/*     EVT_CMD_STATUS   = 0x0F, */
/*     EVT_CMD_COMPLETE = 0x0E, */
/* } event_types_id; */
typedef enum {
#define X(name, byte, type) name = byte,
    EVENT_TYPES
#undef X
} event_types_id;


/* Union-backed packet */
/* typedef struct { */
/*     event_types_id type; */
/*     union { */
/*         struct evt_cmd_status   EVT_CMD_STATUS_data; */
/*         struct evt_cmd_complete EVT_CMD_COMPLETE_data; */
/*     }; */
/* } event_pkt; */
typedef struct {
    event_types_id type;
    union {
#define X(name, byte, type) type name##_data;
        EVENT_TYPES
#undef X
    };
} event_pkt;



/* Factory allocator */
/* void *event_alloc(uint8_t byte) */
/* { */
/*     switch (byte) { */
/*         case 0x0F: return malloc(sizeof(struct evt_cmd_status)); */
/*         case 0x0E: return malloc(sizeof(struct evt_cmd_complete)); */
/*     } */
/*     return NULL; */
/* } */
void *event_alloc(uint8_t byte)
{
    switch (byte) {
#define X(name, byte, type) case byte: return malloc(sizeof(type));
        EVENT_TYPES
#undef X
    }
    return NULL;
}



int main(void)
{
    uint8_t evt = 0x0F;

    struct evt_cmd_status *s =
        (struct evt_cmd_status *)event_alloc(evt);

    if (!s)
        return 1;

    s->status = 0xAA;
    printf("status = 0x%02X\n", s->status);

    free(s);
    return 0;
}
