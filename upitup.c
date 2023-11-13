#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <tuple>

#include "queue.hpp"

struct board {
    int e[3][3];
};

long ord(const board& b){
    long o = 0;
    int pow = 1;
    for (int r = 2; r >= 0; --r) {
        for (int c = 2; c >= 0; --c) {
            o += b.e[r][c] * pow;
            pow *= 7;
        }
    }
    return o;
}

void read_board(board& b)
{
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            scanf("%d", &b.e[r][c]);
        }
    }
}

std::tuple<int, int> find_space(const board& b)
{
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (b.e[r][c] == 0) return { r, c };
    assert(0);
    return { 0 , 0};
}

board up(const board& b)
{
    // const auto [r, c] = find_space(b);
    const std::tuple<int, int> space = find_space(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (r == 2) return b;
    board o = b;
    switch(o.e[r+1][c]){
        case 1: o.e[r][c] = 3; break;
        case 2: o.e[r][c] = 2; break;
        case 3: o.e[r][c] = 6; break;
        case 4: o.e[r][c] = 4; break;
        case 5: o.e[r][c] = 1; break;
        case 6: o.e[r][c] = 5; break;
    }
    o.e[r+1][c] = 0;
    return o;
}

board down(const board& b)
{
    // const auto [r, c] = find_space(b);
    const std::tuple<int, int> space = find_space(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (r == 0) return b;
    board o = b;
    switch(o.e[r-1][c]){
        case 1: o.e[r][c] = 5; break;
        case 2: o.e[r][c] = 2; break;
        case 3: o.e[r][c] = 1; break;
        case 4: o.e[r][c] = 4; break;
        case 5: o.e[r][c] = 6; break;
        case 6: o.e[r][c] = 3; break;
    }
    o.e[r-1][c] = 0;
    return o;
}

board left(const board& b)
{
    // const auto [r, c] = find_space(b);
    const std::tuple<int, int> space = find_space(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (c == 2) return b;
    board o = b;
    switch(o.e[r][c+1]){
        case 1: o.e[r][c] = 2; break;
        case 2: o.e[r][c] = 6; break;
        case 3: o.e[r][c] = 3; break;
        case 4: o.e[r][c] = 1; break;
        case 5: o.e[r][c] = 5; break;
        case 6: o.e[r][c] = 4; break;
    }
    o.e[r][c+1] = 0;
    return o;
}

board right(const board& b)
{
    // const auto [r, c] = find_space(b);
    const std::tuple<int, int> space = find_space(b);
    const int r = std::get<0>(space);
    const int c = std::get<1>(space);
    if (c == 0) return b;
    board o = b;
    switch(o.e[r][c-1]){
        case 1: o.e[r][c] = 4; break;
        case 2: o.e[r][c] = 1; break;
        case 3: o.e[r][c] = 3; break;
        case 4: o.e[r][c] = 6; break;
        case 5: o.e[r][c] = 5; break;
        case 6: o.e[r][c] = 2; break;
    } 
    o.e[r][c-1] = 0;
    return o;
}

bool is_same(const board& a, const board &b)
{
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (a.e[r][c] != b.e[r][c]) return false;

    return true;
}

enum move { L = 1, R = 2, U = 3, D = 4 };

/*
 * Return a shortest path from src to dest.
 */
#define MAX_ORD (40353600)
std::vector<int> solve(const board& src, const board& dest)
{
    queue<board, MAX_ORD> q;
    int visited[MAX_ORD];
    board parent[MAX_ORD];

    enqueue(q, src);
    visited[ord(src)] = L;

    while (!is_empty(q)) {
        board u = dequeue(q);
        if (is_same(u, dest)) {
            /* return the moves to get to u from src. */
            std::vector<int> moves;
            board c = u;
            int o = ord(c);
            while (!is_same(c, src)) {
                moves.push_back(visited[o]);
                c = parent[o];
                o = ord(c);
            }
            std::reverse(moves.begin(), moves.end());
            return moves;
        }

        board a = up(u);
        board b = down(u);
        board c = left(u);
        board d = right(u);

        int aord = ord(a);
        int bord = ord(b);
        int cord = ord(c);
        int dord = ord(d);
        
        if (!visited[aord]) {
            visited[aord] = U;
            parent[aord] = u;
            enqueue(q, a);
        }
        if (!visited[bord]) {
            visited[bord] = D;
            parent[bord] = u;
            enqueue(q, b);
        }
        if (!visited[cord]) {
            visited[cord] = L;
            parent[cord] = u;
            enqueue(q, c);
        }
        if (!visited[dord]) {
            visited[dord] = R;
            parent[dord] = u;
            enqueue(q, d);
        }
    }

    printf("\nBoard is not solvable\n");
    return{0};
}

void print_moves(const std::vector<int>& moves)
{
    for (auto m: moves) {
        switch (m) {
        case L: printf("L "); break;
        case R: printf("R "); break;
        case U: printf("U "); break;
        case D: printf("D "); break;
        }
    }
    printf("\n");
}

int main()
{
    board src, dest;

    read_board(src);
     for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            dest.e[r][c]=1;
        }
    }
    dest.e[1][1]=0;
    
    auto moves = solve(src, dest);
    print_moves(moves);

    return 0;
}
