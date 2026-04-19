#include <stdio.h>

/* main function */
int main(int argc, char *argv[]) {
    FILE *address = fopen(argv[1], "r"); /* opening addresses.txt */

    int addr;

    while (fscanf(address, "%d", &addr) == 1) {
        int masked = addr & 0xFFFF; /* rightmost 16 bits masked */

        int pageNum = (masked >> 8) & 0xFF; /* get page number */

        int offset = masked & 0xFF; /* get offset */

        /* output to terminal */
        printf("Logical: %d, Page: %d, Offset: %d\n", masked, pageNum, offset);
    }
    fclose(address);
    return 0;
}