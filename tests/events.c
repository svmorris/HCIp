#include <stdio.h>
#include <../include/hcip.h>



int status_evt();


int main(int argc, char **argv)
{
    status_evt();
    return 0;
}


int status_evt()
{
    hci_pkt pkt = {0};
    int ret = 0;
    uint8_t buffer[] = { 0x04, 0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x0d, 0x85, 0x30, 0x0d, 0x02, 0x00, 0x85, 0x30, };
    if ((ret = parse_hci(buffer, sizeof(buffer), pkt)) < 0)
        fprintf(stderr, "Failed to parse HCI command");

    printf("Parser returned with code: %x", ret);
    return 0;
}
