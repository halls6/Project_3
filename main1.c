#include <stdio.h>
#include <string.h>

/* constants */
#define PAGE_SIZE 256
#define FRAME_COUNT 256
#define TLB_SIZE 16
#define PAGE_COUNT 256

signed char physicalMem[FRAME_COUNT][PAGE_SIZE]; /* physical memory */

/* arrays */
int pageTable[PAGE_COUNT]; 
int TLBPage[TLB_SIZE];
int TLBFrame[TLB_SIZE];

int TLBNext = 0;

/* counters */
int pageFaults = 0;
int tableHits = 0;
int nextFram = 0;
int total = 0;

/* main function */
int main(int argc, char *argv[]) {
    int i;

    /* page table and TLB initialized to -1 */
    for (int i = 0; i < PAGE_COUNT; i++) {
        pageTable[i] = -1;
    }
    for (int i = 0; i < TLB_SIZE; i++) {
        TLBPage[i] = -1;
        TLBFrame[i] = -1;
    }

    /* reading in files */
    FILE *address = fopen(argv[1], "r"); /* opening addresses.txt */
    FILE *backing = fopen("BACKING_STORE.bin", "rb"); /* opening backing file */
    
    /* output files */
    FILE *out1 = fopen("out1.txt", "w");
    FILE *out2 = fopen("out2.txt", "w");
    FILE *out3 = fopen("out3.txt", "w");
    
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