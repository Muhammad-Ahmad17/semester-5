#include <stdio.h>
#include <stdlib.h>

/*
 * Task 2 – LRU page replacement with dynamic input.
 *   (a) Source is thoroughly commented to explain LRU behaviour.
 *   (b) Program prints hit/miss counts and ratios for any frame size
 *       (e.g., 3) using the same reference string as Task 1 by default.
 *   (c) Users may pass an arbitrary-length page string via the command line:
 *         ./task2 <capacity> <p0> <p1> ...
 */

typedef struct {
    int hits;
    int misses;
} Metrics;

static void print_stats(const Metrics *m, int total) {
    double hit_ratio = (total == 0) ? 0.0 : (double) m->hits / total;
    double miss_ratio = (total == 0) ? 0.0 : (double) m->misses / total;

    printf("Hits       : %d\n", m->hits);
    printf("Misses     : %d\n", m->misses);
    printf("Hit ratio  : %.4f\n", hit_ratio);
    printf("Miss ratio : %.4f\n", miss_ratio);
}

static void simulate_lru(const int *pages, int length, int capacity) {
    int *frames = (int *) malloc(sizeof(int) * capacity);
    int *last_used = (int *) malloc(sizeof(int) * capacity);
    Metrics m = {0, 0};
    int clock = 0;          /* logical timestamp increments on every access */

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    for (int i = 0; i < length; ++i) {
        int page = pages[i];
        int hit_index = -1;

        /* Linear search to see if the page already resides in a frame */
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == page) {
                hit_index = j;
                break;
            }
        }

        if (hit_index != -1) {
            last_used[hit_index] = ++clock;  /* refresh timestamp */
            m.hits++;
            continue;
        }

        /* Miss: locate either a free frame or the least recently used one */
        int victim = -1;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == -1) {
                victim = j;      /* empty slot */
                break;
            }
        }
        if (victim == -1) {
            victim = 0;
            for (int j = 1; j < capacity; ++j) {
                if (last_used[j] < last_used[victim]) {
                    victim = j;
                }
            }
        }

        frames[victim] = page;
        last_used[victim] = ++clock;
        m.misses++;
    }

    printf("LRU simulation with frame size = %d\n", capacity);
    print_stats(&m, length);

    free(frames);
    free(last_used);
}

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <capacity> [page0 page1 ...]\n", prog);
}

int main(int argc, char *argv[]) {
    const int default_pages[] = {1, 3, 0, 3, 5, 6, 3};
    const int default_len = sizeof(default_pages) / sizeof(default_pages[0]);

    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    int capacity = atoi(argv[1]);
    if (capacity <= 0) {
        fprintf(stderr, "Frame size must be positive.\n");
        return 1;
    }

    int *pages = NULL;
    int length = 0;

    if (argc > 2) {
        length = argc - 2;
        pages = (int *) malloc(sizeof(int) * length);
        for (int i = 0; i < length; ++i) {
            pages[i] = atoi(argv[i + 2]);
        }
    } else {
        pages = (int *) malloc(sizeof(int) * default_len);
        for (int i = 0; i < default_len; ++i) {
            pages[i] = default_pages[i];
        }
        length = default_len;
    }

    simulate_lru(pages, length, capacity);
    free(pages);
    return 0;
}
