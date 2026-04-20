#include <stdio.h>
#include <string.h>

/* constants */
#define PAGE_SIZE 256
#define FRAME_COUNT 128
#define TLB_SIZE 16
#define PAGE_COUNT 256

signed char physicalMem[FRAME_COUNT][PAGE_SIZE]; /* physical memory */

/* arrays */
int pageTable[PAGE_COUNT]; 
int TLBPage[TLB_SIZE];
int TLBFrame[TLB_SIZE];
int FIFO_queue[FRAME_COUNT]; /* track page replacement */

/* FIFO */
int TLBNext = 0;
int FIFO_head;
int FIFO_tail = 0;

/* counters */
int pageFaults = 0;
int TLBHits = 0;
int nextFrame = 0;
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

    int logAddr;

    /* logical addresses from file */
    while (fscanf(address, "%d", &logAddr) == 1) {
        total++;
        int masked = logAddr & 0xFFFF; /* rightmost 16 bits masked */
        int pageNum = (masked >> 8) & 0xFF; /* get page number */
        int offset = masked & 0xFF; /* get offset */
        int frameNum = -1;

        /* search TLB for page num */
        for (i = 0; i < TLB_SIZE; i++) {
            if (TLBPage[i] == pageNum) {
                frameNum = TLBFrame[i];
                TLBHits++;
                break;
            }
        }

        /* TLB miss */
        if (frameNum == -1) {
            if (pageTable[pageNum] == -1) {
                pageFaults++; /* page fault to load from backing */
                
                int frame;

                if (nextFrame < FRAME_COUNT) { /* free frames available */
                    frame = nextFrame++;
                }
                else { /* no fre frames */
                    int evictedPage = FIFO_queue[FIFO_head];
                    frame = pageTable[evictedPage];

                    pageTable[evictedPage] = -1;

                    for (i = 0; i < TLB_SIZE; i++) {
                        if (TLBPage[i] == evictedPage) {
                            TLBPage[i] = -1;
                            TLBFrame[i] = -1;
                            break;
                        }
                    }
                    /* FIFO head to next oldest page */
                    FIFO_head = (FIFO_head + 1) % FRAME_COUNT;
                }

                fseek(backing, pageNum * PAGE_SIZE, SEEK_SET);
                fread(physicalMem[frame], sizeof(signed char), PAGE_SIZE, backing);
                pageTable[pageNum] = frame;
                FIFO_queue[FIFO_tail] = pageNum;
                FIFO_tail = (FIFO_tail + 1) % FRAME_COUNT;
            }
            frameNum = pageTable[pageNum];

            /* update TLB */
            TLBPage[TLBNext] = pageNum;
            TLBFrame[TLBNext] = frameNum;
            TLBNext = (TLBNext + 1) % TLB_SIZE;
        }

        int physicalAddress = frameNum * PAGE_SIZE + offset;
        signed char value = physicalMem[frameNum][offset];

        fprintf(out1, "%d\n", logAddr);
        fprintf(out2, "%d\n", physicalAddress);
        fprintf(out3, "%d\n", value);

    }

        /* output to terminal */
    printf("Page faults: %d / %d, %.2f\n", pageFaults, total, (double)pageFaults / total);
    printf("TLB hits: %d / %d, %.2f\n", TLBHits, total, (double)TLBHits / total);
    
    fclose(address);
    fclose(backing);
    fclose(out1);
    fclose(out2);
    fclose(out3);
    
    return 0;
}