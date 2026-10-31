#include <stdio.h>

#define ROWS 10
#define COLS 10

extern int grid[ROWS][COLS];

// Each cell in the BFS queue stores its position + where it came from
typedef struct {
    int r, c;
    int from_r, from_c;
} Cell;

int bfs(int sr, int sc, int er, int ec, int path[][2]) {
    int visited[ROWS][COLS] = {0};
    Cell queue[ROWS * COLS];
    int head = 0, tail = 0;

    // Track where each cell was reached from
    int from_r[ROWS][COLS], from_c[ROWS][COLS];
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            from_r[i][j] = from_c[i][j] = -1;

    queue[tail++] = (Cell){sr, sc, -1, -1};
    visited[sr][sc] = 1;

    // 4 directions: up, down, left, right
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (head < tail) {
        Cell cur = queue[head++];

        // Reached the goal — trace back the path
        if (cur.r == er && cur.c == ec) {
            int len = 0;
            int tr = er, tc = ec;
            // Count path length first
            while (tr != -1) {
                len++;
                int pr = from_r[tr][tc];
                int pc = from_c[tr][tc];
                tr = pr; tc = pc;
            }
            // Fill path array in correct order
            tr = er; tc = ec;
            for (int i = len - 1; i >= 0; i--) {
                path[i][0] = tr;
                path[i][1] = tc;
                int pr = from_r[tr][tc];
                int pc = from_c[tr][tc];
                tr = pr; tc = pc;
            }
            return len;
        }

        // Explore neighbors
        for (int d = 0; d < 4; d++) {
            int nr = cur.r + dr[d];
            int nc = cur.c + dc[d];
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS
                && grid[nr][nc] == 0 && !visited[nr][nc]) {
                visited[nr][nc] = 1;
                from_r[nr][nc] = cur.r;
                from_c[nr][nc] = cur.c;
                queue[tail++] = (Cell){nr, nc, cur.r, cur.c};
            }
        }
    }
    return -1; // no path found
}
