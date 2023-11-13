#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Define cube configurations, moves, and representation
#define NMOVES 6
#define NCONFIGS 40320 // 8! * 3^8

char moves[NMOVES] = {'R', 'U', 'F', 'R\'', 'U\'', 'F\''};
int fact[8] = {1, 1, 2, 6, 24, 120, 720, 5040};

// Represent the cube as a single integer
struct Cube {
    int perm, orien;
};

struct Cube queue[NCONFIGS];
int head, tail;
bool visited[NCONFIGS];
int from[NCONFIGS];
char movesDone[NCONFIGS];

void init() {
    head = tail = 0;
    memset(visited, 0, sizeof(visited));
}

int permute(int perm, int move) {
    int i, j, temp, res = 0;
    for (i = 0; i < 8; i++) {
        j = perm / fact[7 - i];
        temp = perm % fact[7 - i];
        if (i == 0 && moves[move] <= 'Z') j = (j + 1) % 3;
        if (i == 1 && moves[move] <= 'Z') j = (j + 2) % 3;
        res = res * fact[7 - i] + j;
        perm = temp;
    }
    return res;
}

void push(int perm, int orien) {
    int index = perm * 3 + orien;
    queue[tail].perm = perm;
    queue[tail].orien = orien;
    tail++;
    visited[index] = true;
}

void solve(int perm, int orien) {
    init();
    push(perm, orien);
    int i, nextPerm, nextOrien;
    while (head < tail) {
        struct Cube curr = queue[head];
        int currPerm = curr.perm, currOrien = curr.orien;
        if (currPerm == 0 && currOrien == 0) {
            break;
        }
        for (i = 0; i < NMOVES; i++) {
            nextPerm = permute(currPerm, i);
            nextOrien = (currOrien + (moves[i] > 'Z')) % 3;
            int nextIndex = nextPerm * 3 + nextOrien;
            if (!visited[nextIndex]) {
                visited[nextIndex] = true;
                from[nextIndex] = head;
                movesDone[nextIndex] = moves[i];
                push(nextPerm, nextOrien);
            }
        }
        head++;
    }
    // Reconstruction of the solution path
    int solutionMoves = 0;
    while (head > 0) {
        head = from[head * 3 + (head + 1) % 3];
        solutionMoves++;
    }
    printf("Shortest solution in %d moves: ", solutionMoves);
    while (tail > 0) {
        tail = from[tail * 3 + (tail + 1) % 3];
        if (tail != 0)
            printf("%c ", movesDone[tail]);
    }
}

int main() {
    int initialPerm = 0; // Input the initial configuration
    int initialOrien = 0; // Input the initial orientation
    solve(initialPerm, initialOrien);
    return 0;
}
