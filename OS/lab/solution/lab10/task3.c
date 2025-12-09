#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/*
 * Task 3 – Optimal page replacement (with optional FIFO/LRU comparison).
 *  (a) Thorough inline comments walk through Optimal's look-ahead logic.
 *  (b) Program reports hit/miss metrics and, when invoked with --compare,
 *      executes FIFO and LRU as well for side-by-side ratios.
 *  (c) Run the executable with frame size 3 and 4 to study the effect of
 *      larger memory (Belady's anomaly does not appear for this string).
 */

typedef struct {
    int hits;
    int misses;
} Metrics;

static void print_stats(const char *label, Metrics m, int total) {
    double hit_ratio = (total == 0) ? 0.0 : (double) m.hits / total;
    double miss_ratio = (total == 0) ? 0.0 : (double) m.misses / total;

    printf("%s\n", label);
    printf("  Hits       : %d\n", m.hits);
    printf("  Misses     : %d\n", m.misses);
    printf("  Hit ratio  : %.4f\n", hit_ratio);
    printf("  Miss ratio : %.4f\n\n", miss_ratio);
}

static Metrics simulate_fifo(const int *pages, int length, int capacity) {
    Metrics m = {0, 0};
    int *frames = (int *) malloc(sizeof(int) * capacity);
    int front = 0;

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;
    }

    for (int i = 0; i < length; ++i) {
        int page = pages[i];
        int hit = 0;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == page) {
                hit = 1;
                break;
            }
        }
        if (hit) {
            m.hits++;
            continue;
        }
        frames[front] = page;
        front = (front + 1) % capacity;
        m.misses++;
    }

    free(frames);
    return m;
}

static Metrics simulate_lru(const int *pages, int length, int capacity) {
    Metrics m = {0, 0};
    int *frames = (int *) malloc(sizeof(int) * capacity);
    int *last_used = (int *) malloc(sizeof(int) * capacity);
    int clock = 0;

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    for (int i = 0; i < length; ++i) {
        int page = pages[i];
        int idx = -1;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == page) {
                idx = j;
                break;
            }
        }
        if (idx != -1) {
            last_used[idx] = ++clock;
            m.hits++;
            continue;
        }

        int victim = -1;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == -1) {
                victim = j;
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

    free(frames);
    free(last_used);
    return m;
}

static Metrics simulate_optimal(const int *pages, int length, int capacity) {
    Metrics m = {0, 0};
    int *frames = (int *) malloc(sizeof(int) * capacity);

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;
    }

    for (int i = 0; i < length; ++i) {
        int page = pages[i];
        int idx = -1;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == page) {
                idx = j;
                break;
            }
        }
        if (idx != -1) {
            m.hits++;
            continue;
        }

        int victim = -1;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == -1) {
                victim = j;       /* free frame available */
                break;
            }
        }
        if (victim == -1) {
            int farthest = -1;    /* index of farthest future use */
            for (int j = 0; j < capacity; ++j) {
                int next_use = INT_MAX;
                for (int k = i + 1; k < length; ++k) {
                    if (frames[j] == pages[k]) {
                        next_use = k;
                        break;    /* first future occurrence wins */
                    }
                }
                if (next_use > farthest) {
                    farthest = next_use;
                    victim = j;
                }
            }
        }

        frames[victim] = page;
        m.misses++;
    }

    free(frames);
    return m;
}

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s [--compare] <capacity> [page0 page1 ...]\n", prog);
}

int main(int argc, char *argv[]) {
    const int default_pages[] = {1, 3, 0, 3, 5, 6, 3};
    const int default_len = sizeof(default_pages) / sizeof(default_pages[0]);

    int compare = 0;
    int arg_index = 1;
    if (argc > 1 && strcmp(argv[1], "--compare") == 0) {
        compare = 1;
        arg_index++;
    }

    if (argc <= arg_index) {
        usage(argv[0]);
        return 1;
    }

    int capacity = atoi(argv[arg_index++]);
    if (capacity <= 0) {
        fprintf(stderr, "Frame size must be positive.\n");
        return 1;
    }

    int *pages = NULL;
    int length = 0;

    if (argc > arg_index) {
        length = argc - arg_index;
        pages = (int *) malloc(sizeof(int) * length);
        for (int i = 0; i < length; ++i) {
            pages[i] = atoi(argv[arg_index + i]);
        }
    } else {
        length = default_len;
        pages = (int *) malloc(sizeof(int) * length);
        for (int i = 0; i < length; ++i) {
            pages[i] = default_pages[i];
        }
    }

    Metrics opt = simulate_optimal(pages, length, capacity);
    print_stats("Optimal", opt, length);

    if (compare) {
        Metrics fifo = simulate_fifo(pages, length, capacity);
        Metrics lru = simulate_lru(pages, length, capacity);
        print_stats("FIFO", fifo, length);
        print_stats("LRU", lru, length);
    }

    free(pages);
    return 0;
}
