#include <stdio.h>
#include <string.h>

#define ROWS 10
#define COLS 10
#define MAX_PATH 200
#define NUM_ROBOTS 2

extern int grid[ROWS][COLS];
int bfs(int sr, int sc, int er, int ec, int path[][2]);

typedef struct {
    int id;
    int r, c;
    int path[MAX_PATH][2];
    int path_len;
    int step;
    int waiting;
    int done;
} Robot;

// Which robot ID currently occupies each cell (-1 = empty)
int occupied[ROWS][COLS];

void init_robots(Robot robots[]) {
    // Robot 1: top-left to bottom-right
    robots[0].id = 1;
    robots[0].r = 1; robots[0].c = 1;
    robots[0].step = 0; robots[0].waiting = 0; robots[0].done = 0;
    robots[0].path_len = bfs(1, 1, 8, 8, robots[0].path);

    // Robot 2: top-right to bottom-left
    robots[1].id = 2;
    robots[1].r = 1; robots[1].c = 8;
    robots[1].step = 0; robots[1].waiting = 0; robots[1].done = 0;
    robots[1].path_len = bfs(1, 8, 8, 1, robots[1].path);
}

void print_state(Robot robots[], int step) {
    printf("\n--- Step %d ---\n\n", step);

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (grid[r][c] == 1) {
                printf("# ");
            } else {
                int found = 0;
                for (int i = 0; i < NUM_ROBOTS; i++) {
                    if (!robots[i].done &&
                        robots[i].r == r && robots[i].c == c) {
                        printf("%d ", robots[i].id);
                        found = 1;
                        break;
                    }
                }
                if (!found) printf(". ");
            }
        }
        printf("\n");
    }

    // Status line
    for (int i = 0; i < NUM_ROBOTS; i++) {
        if (robots[i].done)
            printf("  Robot %d: DONE\n", robots[i].id);
        else if (robots[i].waiting)
            printf("  Robot %d: waiting at (%d,%d)\n",
                   robots[i].id, robots[i].r, robots[i].c);
        else
            printf("  Robot %d: at (%d,%d)\n",
                   robots[i].id, robots[i].r, robots[i].c);
    }
}

void run_simulation(Robot robots[]) {
    int step = 0;
    print_state(robots, step);

    while (1) {
        // Check if all robots are done
        int all_done = 1;
        for (int i = 0; i < NUM_ROBOTS; i++)
            if (!robots[i].done) { all_done = 0; break; }
        if (all_done) break;

        // Build occupied map from current positions
        memset(occupied, -1, sizeof(occupied));
        for (int i = 0; i < NUM_ROBOTS; i++)
            if (!robots[i].done)
                occupied[robots[i].r][robots[i].c] = robots[i].id;

        // Each robot tries to claim its next cell
        int next_r[NUM_ROBOTS], next_c[NUM_ROBOTS];
        int claimed[ROWS][COLS];
        memset(claimed, -1, sizeof(claimed));

        for (int i = 0; i < NUM_ROBOTS; i++) {
            if (robots[i].done) continue;

            if (robots[i].step >= robots[i].path_len) {
                robots[i].done = 1;
                continue;
            }

            next_r[i] = robots[i].path[robots[i].step][0];
            next_c[i] = robots[i].path[robots[i].step][1];

            if (claimed[next_r[i]][next_c[i]] == -1) {
                // Cell is free — claim it
                claimed[next_r[i]][next_c[i]] = i;
                robots[i].waiting = 0;
            } else {
                // Collision — robot with lower ID wins, other waits
                int other = claimed[next_r[i]][next_c[i]];
                if (i < other) {
                    // This robot wins, bump the other
                    claimed[next_r[i]][next_c[i]] = i;
                    robots[other].waiting = 1;
                } else {
                    robots[i].waiting = 1;
                }
            }
        }

        // Apply moves
        for (int i = 0; i < NUM_ROBOTS; i++) {
            if (robots[i].done || robots[i].waiting) continue;
            robots[i].r = next_r[i];
            robots[i].c = next_c[i];
            robots[i].step++;
        }

        step++;
        print_state(robots, step);

        if (step > 100) { printf("Max steps reached.\n"); break; }
    }

    printf("\n=== Simulation complete in %d steps ===\n", step);
}

void start_simulation() {
    Robot robots[NUM_ROBOTS];
    init_robots(robots);
    run_simulation(robots);
}


