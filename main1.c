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
int TLBHits = 0;
int nextFrame = 0;
int total = 0;

/* main function */
int main(int argc, char *argv[]) {
    int i;

    /* req5: page table and TLB initialized to -1 */
    for (int i = 0; i < PAGE_COUNT; i++) {
        pageTable[i] = -1;
    }

    /* req4 */
    for (int i = 0; i < TLB_SIZE; i++) {
        TLBPage[i] = -1;
        TLBFrame[i] = -1;
    }

    /* REQUIREMENT 1: correctly read in input logical addresses */
    /* reading in files */
    FILE *address = fopen(argv[1], "r"); /* opening addresses.txt */
    FILE *backing = fopen("BACKING_STORE.bin", "rb"); /* opening backing file */
    
    /* output files */
    FILE *out1 = fopen("out1.txt", "w");
    FILE *out2 = fopen("out2.txt", "w");
    FILE *out3 = fopen("out3.txt", "w");

    int logAddr;

    /* logical addresses from files */
    while (fscanf(address, "%d", &logAddr) == 1) {
        total++;
        /* REQUIREMENT 2: correctly translated input addresses to physical */
        int masked = logAddr & 0xFFFF; /* rightmost 16 bits masked */
        int pageNum = (masked >> 8) & 0xFF; /* get page number */
        int offset = masked & 0xFF; /* get offset */
        int frameNum = -1;

        /* REQUIREMENT 4: implement FIFO based TLB update */
        /* search TLB for page num */
        for (i = 0; i < TLB_SIZE; i++) {
            if (TLBPage[i] == pageNum) {
                frameNum = TLBFrame[i];
                TLBHits++; /* Requirement 6: count the number of TLB hits */
                break;
            }
        }

        /* TLB miss */
        if (frameNum == -1) {
            /* REQUIREMENT 5: counter number of page faults */
            if (pageTable[pageNum] == -1) {
                pageFaults++; /* page fault to load from backing */
            
                /* req 3: physical memory loaded */
                fseek(backing, pageNum * PAGE_SIZE, SEEK_SET);
                fread(physicalMem[nextFrame], sizeof(signed char), PAGE_SIZE, backing);
                pageTable[pageNum] = nextFrame;
                nextFrame++;
            }
            frameNum = pageTable[pageNum];

            /* req4: update TLB */
            TLBPage[TLBNext] = pageNum;
            TLBFrame[TLBNext] = frameNum;
            TLBNext = (TLBNext + 1) % TLB_SIZE;
        }

        /* req2: translated */
        int physicalAddress = frameNum * PAGE_SIZE + offset;

        /* req3: retrieved values */
        signed char value = physicalMem[frameNum][offset];

        /* write to output files */
        fprintf(out1, "%d\n", logAddr);
        fprintf(out2, "%d\n", physicalAddress); /* req2 */
        fprintf(out3, "%d\n", value); /* req3 */

    }

    /* output to terminal */
    printf("Page faults: %d / %d, %.2f\n", pageFaults, total, (double)pageFaults / total);
    printf("TLB hits: %d / %d, %.2f\n", TLBHits, total, (double)TLBHits / total);
    
    /* close files */
    fclose(address);
    fclose(backing);
    fclose(out1);
    fclose(out2);
    fclose(out3);
    
    return 0;
}