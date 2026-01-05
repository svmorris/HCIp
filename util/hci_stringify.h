#ifndef PARSER_HCI_STRINGIFY
#define PARSER_HCI_STRINGIFY

#include <unistd.h>

// forward declaration for hcip.h
// struct hci_pkt;
typedef struct hci_pkt hci_pkt;

/*
 * How long should the buffer be?
 *
 * At the moment the caller has to provide
 * a buffer that is longer than the maximum
 * amount of data stringify can write.
 * For reference it can use the HCI_STRINGIFY_BUF_LEN
 * variable.
 *
 * This is obviously not a good solution.
 * HCI_STRINGIFY_BUF_LEN is probably way more than
 * any buffer will need. Additionally its stupid
 * to assume the caller will know the output length.
 *
 * However, this is a solution designed with the
 * single purpose of getting this project to move
 * forward. Instead of spending time on variable
 * buffers when it is not the important task.
 *
 * The struct based buffer is designed so that
 * in the future, this function can be updated
 * with a more correct, growing buffer.
 */

#define HCI_STRINGIFY_BUF_LEN 4098

typedef struct {
    char *buf;
    size_t len; // number of bytes written.
    size_t cap; // length of buffer.
}hci_strbuf;

int hci_stringify(hci_pkt *pkt, hci_strbuf *out);


typedef enum {
    S_OK = 0,
    S_INVALID = 1,     // Invalid data in hci_pkt.
    S_OUTOFBOUNDS = 2, // Buffer not long enough.
    S_INTERNALERR = 3, // Internal error.
}stringify_errors;



#endif
