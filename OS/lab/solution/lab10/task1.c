#include <stdio.h>
#include <stdlib.h>

/*
 * Task 1 – FIFO page replacement.
 *  (a) Code is fully commented below to explain the control flow.
 *  (b) Program prints hit/miss counts and ratios for the supplied frame size.
 *  (c) To study the impact of frame size, simply pass a different capacity on
 *      the command line (e.g., 3 or 4) and compare the ratios.
 */

/* Utility to pretty-print the metrics. */
static void print_stats(int hits, int misses, int total) {
    const double hit_ratio = (total == 0) ? 0.0 : (double) hits / total;
    const double miss_ratio = (total == 0) ? 0.0 : (double) misses / total;

    printf("Hits       : %d\n", hits);
    printf("Misses     : %d\n", misses);
    printf("Hit ratio  : %.4f\n", hit_ratio);
    printf("Miss ratio : %.4f\n", miss_ratio);
}

/* FIFO simulation for a fixed reference string. */
static void simulate_fifo(const int *pages, int length, int capacity) {
    int frames[8];                /* enough space for the small lab capacity */
    int front = 0;                /* points to the oldest page currently in memory */
    int hits = 0, misses = 0;

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;           /* mark every frame as empty */
    }

    for (int i = 0; i < length; ++i) {
        int page = pages[i];
        int found = 0;

        /* search the frame array for the current page */
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == page) {
                found = 1;        /* hit: page already loaded */
                break;
            }
        }

        if (found) {
            hits++;
            continue;
        }

        /* miss: overwrite the oldest entry (pointed to by 'front'). */
        frames[front] = page;
        front = (front + 1) % capacity;  /* wrap the pointer in circular order */
        misses++;
    }

    printf("FIFO simulation with frame size = %d\n", capacity);
    print_stats(hits, misses, length);
}

int main(int argc, char *argv[]) {
    /* reference string shared by the lab handout */
    const int pages[] = {1, 3, 0, 3, 5, 6, 3};
    const int length = sizeof(pages) / sizeof(pages[0]);

    int capacity = 3;                 /* default for Task 1(b) */
    if (argc > 1) {
        capacity = atoi(argv[1]);
        if (capacity <= 0 || capacity > 8) {
            fprintf(stderr, "Invalid frame size '%s'.\n", argv[1]);
            return 1;
        }
    }

    simulate_fifo(pages, length, capacity);
    return 0;
}
