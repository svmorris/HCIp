#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <../include/hcip.h>
#include <../util/hci_stringify.h>



void status_evt();
void cmd_complete_evt();


int main()
{
    puts("-----------------------------");
    puts("testing cmd status");
    status_evt();
    puts("-----------------------------");
    puts("testing cmd complete");
    cmd_complete_evt();
    return 0;
}


void cmd_complete_evt()
{
    hci_pkt pkt = {0};
    int ret = 0;
    uint8_t buffer[] = { 0x04, 0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x0d, 0x85, 0x30, 0x0d, 0x02, 0x00, 0x85, 0x30 };
    if ((ret = parse_hci(buffer, sizeof(buffer), pkt)) > 0)
        fprintf(stderr, "Parser failed with exit code 0x%x\n", ret);

    char *str = malloc(HCI_STRINGIFY_BUF_LEN);
    if (str == NULL)
    {
        perror("Buffer allocation");
        return;
    }

    hci_strbuf strbuf = {
        .buf = str,
        .len = 0,
        .cap = HCI_STRINGIFY_BUF_LEN
    };

    if ((ret = hci_stringify(&pkt, &strbuf)) != 0)
        fprintf(stderr, "Stringify failed with exit code 0x%x.\n", ret);

    printf("Parsed command: %s\n", strbuf.buf);
}

void status_evt()
{
    hci_pkt pkt = {0};
    int ret = 0;
    uint8_t buffer[] = {0x04, 0x0f, 0x04, 0x01, 0x01, 0x0c, 0x02};

    if ((ret = parse_hci(buffer, sizeof(buffer), pkt)) > 0)
        fprintf(stderr, "Parser failed with exit code 0x%x\n", ret);



    char *str = malloc(HCI_STRINGIFY_BUF_LEN);
    if (str == NULL)
    {
        perror("Buffer allocation");
        return;
    }

    hci_strbuf strbuf = {
        .buf = str,
        .len = 0,
        .cap = HCI_STRINGIFY_BUF_LEN
    };

    if ((ret = hci_stringify(&pkt, &strbuf)) != 0)
        fprintf(stderr, "Stringify failed with exit code 0x%x.\n", ret);

    printf("Parsed command: %s\n", strbuf.buf);
}
