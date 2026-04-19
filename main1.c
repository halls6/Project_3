#include <stdio.h>
#include <string.h>

/* constants */
#define PAGE_SIZE 256
#define FRAME_COUNT 256
#define TABLE_SIZE 16
#define PAGE_COUNT 256

signed char physicalMem[FRAME_COUNT][PAGE_SIZE]; /* physical memory */

/* tables */
int pageTable[PAGE_COUNT]; 
int tablePage[TABLE_SIZE];
int tableFrame[TABLE_SIZE];
int tableNext = 0;

/* counters */
int pageFaults = 0;
int tableHits = 0;
int nextFram = 0;
int total = 0;

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