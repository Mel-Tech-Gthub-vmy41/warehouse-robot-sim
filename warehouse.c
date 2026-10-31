#include <stdio.h>
#include <string.h>

#define ROWS 10
#define COLS 10

int grid[ROWS][COLS];

extern void start_simulation();

// Simple JSON parser — reads grid and robot config from stdin
void load_config() {
    char line[256];
    int row = 0;
    int in_grid = 0;

    while (fgets(line, sizeof(line), stdin)) {
        // Detect grid section
        if (strstr(line, "\"grid\"")) { in_grid = 1; continue; }

        if (in_grid && row < ROWS) {
            // Parse a row like: [1,0,0,0,1,0,0,0,0,1],
            int col = 0;
            for (int i = 0; line[i] != '\0' && col < COLS; i++) {
                if (line[i] == '0' || line[i] == '1') {
                    grid[row][col++] = line[i] - '0';
                }
            }
            if (col == COLS) row++;
        }

        // Stop reading grid after 10 rows
        if (row == ROWS) in_grid = 0;
    }
}

int main() {
    load_config();
    start_simulation();
    return 0;
}
