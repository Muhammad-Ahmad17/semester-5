#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
 * Small helper container to keep every simulation metric together.
 */
typedef struct {
    int hits;
    int misses;
} SimResult;

static int contains(const int *frames, int capacity, int page) {
    for (int i = 0; i < capacity; ++i) {
        if (frames[i] == page) {
            return i;
        }
    }
    return -1;
}

static void print_summary(const char *name, SimResult result, int total) {
    double hit_ratio = total ? (double) result.hits / total : 0.0;
    double miss_ratio = total ? (double) result.misses / total : 0.0;
    printf("%s\n", name);
    printf("  Hits        : %d\n", result.hits);
    printf("  Misses      : %d\n", result.misses);
    printf("  Hit ratio   : %.4f\n", hit_ratio);
    printf("  Miss ratio  : %.4f\n", miss_ratio);
}

/*
 * First-In First-Out (FIFO) page replacement.
 * The oldest page in the frame set is tracked by the rotating 'front' index.
 */
SimResult simulate_fifo(const int *pages, int n, int capacity) {
    SimResult res = {0, 0};
    int *frames = (int *) malloc(sizeof(int) * capacity);
    int front = 0;

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;    /* -1 indicates an empty frame */
    }

    for (int i = 0; i < n; ++i) {
        int idx = contains(frames, capacity, pages[i]);
        if (idx != -1) {
            res.hits++;
            continue;
        }

        /* Replace the page that has been in memory the longest */
        frames[front] = pages[i];
        front = (front + 1) % capacity;
        res.misses++;
    }

    free(frames);
    return res;
}

/*
 * Least Recently Used (LRU).
 * Each frame stores a time stamp (logical clock) telling when it was last accessed.
 * On a miss we evict the frame with the smallest time stamp (i.e., used farthest back).
 */
SimResult simulate_lru(const int *pages, int n, int capacity) {
    SimResult res = {0, 0};
    int *frames = (int *) malloc(sizeof(int) * capacity);
    int *last_used = (int *) malloc(sizeof(int) * capacity);
    int clock = 0;

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    for (int i = 0; i < n; ++i) {
        int idx = contains(frames, capacity, pages[i]);
        if (idx != -1) {
            last_used[idx] = ++clock;  /* refresh time stamp on hit */
            res.hits++;
            continue;
        }

        /* Try to find an empty frame first */
        int victim = -1;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == -1) {
                victim = j;
                break;
            }
        }

        if (victim == -1) {
            /* All frames occupied, evict the least recently used page */
            victim = 0;
            for (int j = 1; j < capacity; ++j) {
                if (last_used[j] < last_used[victim]) {
                    victim = j;
                }
            }
        }

        frames[victim] = pages[i];
        last_used[victim] = ++clock;
        res.misses++;
    }

    free(frames);
    free(last_used);
    return res;
}

/*
 * Optimal page replacement (Belady's algorithm).
 * Looks ahead to replace the frame whose page is needed farthest in the future,
 * or not at all.
 */
SimResult simulate_optimal(const int *pages, int n, int capacity) {
    SimResult res = {0, 0};
    int *frames = (int *) malloc(sizeof(int) * capacity);

    for (int i = 0; i < capacity; ++i) {
        frames[i] = -1;
    }

    for (int i = 0; i < n; ++i) {
        int idx = contains(frames, capacity, pages[i]);
        if (idx != -1) {
            res.hits++;
            continue;
        }

        int victim = -1;
        for (int j = 0; j < capacity; ++j) {
            if (frames[j] == -1) {
                victim = j;  /* empty slot available */
                break;
            }
        }

        if (victim == -1) {
            int farthest = -1;
            for (int j = 0; j < capacity; ++j) {
                int next_use = INT_MAX;
                for (int k = i + 1; k < n; ++k) {
                    if (frames[j] == pages[k]) {
                        next_use = k;
                        break;
                    }
                }
                if (next_use > farthest) {
                    farthest = next_use;
                    victim = j;
                }
            }
        }

        frames[victim] = pages[i];
        res.misses++;
    }

    free(frames);
    return res;
}

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <fifo|lru|optimal|all> <frame_capacity> <page_0> <page_1> ...\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        usage(argv[0]);
        return 1;
    }

    const char *algo = argv[1];
    int capacity = atoi(argv[2]);
    if (capacity <= 0) {
        fprintf(stderr, "Frame capacity must be positive.\n");
        return 1;
    }

    int page_count = argc - 3;
    int *pages = (int *) malloc(sizeof(int) * page_count);
    for (int i = 0; i < page_count; ++i) {
        pages[i] = atoi(argv[i + 3]);
    }

    if (strcmp(algo, "fifo") == 0 || strcmp(algo, "all") == 0) {
        SimResult fifo_res = simulate_fifo(pages, page_count, capacity);
        print_summary("FIFO", fifo_res, page_count);
    }
    if (strcmp(algo, "lru") == 0 || strcmp(algo, "all") == 0) {
        SimResult lru_res = simulate_lru(pages, page_count, capacity);
        print_summary("LRU", lru_res, page_count);
    }
    if (strcmp(algo, "optimal") == 0 || strcmp(algo, "all") == 0) {
        SimResult opt_res = simulate_optimal(pages, page_count, capacity);
        print_summary("Optimal", opt_res, page_count);
    }

    if (strcmp(algo, "fifo") && strcmp(algo, "lru") && strcmp(algo, "optimal") && strcmp(algo, "all")) {
        fprintf(stderr, "Unknown algorithm '%s'.\n", algo);
        free(pages);
        return 1;
    }

    free(pages);
    return 0;
}
