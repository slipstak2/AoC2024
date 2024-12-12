#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <cassert>
#include <queue>
#include <set>
#include <chrono>
#include <functional>
#include <list>

using namespace std;

//#define TEST
#define DAY 12

using INT = long long;

string InputFile() {
    stringstream ss;
    ss << "day" << DAY;
#if defined(TEST)
    ss << "-test";
#endif
    ss << ".txt";
    return ss.str();
}

std::chrono::time_point<std::chrono::steady_clock> Now() {
    return std::chrono::steady_clock::now();
}

std::string TimeGetStr(int64_t nano) {
    double dur = (double)nano / 1e9;
    char buf[256];
    sprintf_s(buf, "%.3f s", dur);
    return buf;
}

void EXPECT_EQ(int lhs, int rhs) {
    if (lhs != rhs) {
        cerr << lhs << " != " << rhs << endl;
        exit(-1);
    }
}

int ROWS;
int COLS;
const char EMPTY = '.';

struct point {
    int row;
    int col;
};

const point U{ -1,0 };
const point D{ 1, 0 };
const point L{ 0, 1 };
const point R{ 0, -1 };

int border_id = 0;
struct cell_borders {
    int ids[4];
    cell_borders() {
        memset(ids, 0, sizeof(ids));
    }
    void add_border(int pos) {
        ids[pos] = ++border_id;

    }
};

struct Dsu {
    Dsu(int n) {
        parent.resize(n + 1);
        size.resize(n + 1);
        for (int id = 1; id <= n; ++id) {
            parent[id] = id;
        }
    }
    int find_set(int x) {
        if (parent[x] == x) {
            return x;
        }
        return parent[x] = find_set(parent[x]);
    }
    void union_set(int x, int y) {
        x = find_set(x);
        y = find_set(y);
        if (x != y) {
            if (size[x] < size[y]) {
                swap(x, y);
            }
            parent[y] = x;
            size[x] += size[y];
        }
    }
    int roots() {
        return set<int>{parent.begin() + 1, parent.end()}.size();
    }

    vector<int> parent;
    vector<int> size;
};


vector<string> m;
vector<vector<int>> used;
vector<vector<cell_borders>> cb;

point delta[4] = { U, D, L, R };
point dir[4][2] = {
    {L, R}, // UP
    {L, R}, // DOWN
    {U, D}, // LEFT
    {U, D}  // RIGHT
};


int PP = 0;
int S = 0;

void dfs(int row, int col, int id) {
    used[row][col] = id;
    char c = m[row][col];
    S++;
    for (int i = 0; i < 4; ++i) {
        int nrow = row + delta[i].row;
        int ncol = col + delta[i].col;
        char nc = m[nrow][ncol];
        if (nc != c && used[nrow][ncol] != id) {
            cb[row][col].add_border(i);
            PP++;
        }
        if (nc == c && used[nrow][ncol] != id) {
            dfs(nrow, ncol, id);
        }
    }
}
void go(Dsu& dsu, int id) {
    for (int row = 1; row <= ROWS; ++row) {
        for (int col = 1; col <= COLS; ++col) {
            if (used[row][col] == id) {
                for (int i = 0; i < 4; ++i) {
                    int b_id = cb[row][col].ids[i];
                    if (b_id != 0) {
                        for (int j = 0; j < 2; ++j) {
                            point delta = dir[i][j];
                            int nrow = row + delta.row;
                            int ncol = col + delta.col;
                            int nb_id = cb[nrow][ncol].ids[i];
                            if (nb_id != 0) {
                                dsu.union_set(b_id, nb_id);
                            }
                        }
                    }
                }
            }
        }
    }
}


int main()
{
    auto start = Now();

    FILE* f = freopen(InputFile().data(), "r", stdin);
    assert(f != nullptr);
    string row;

    while (getline(cin, row)) {
        row = EMPTY + row + EMPTY;
        m.push_back(row);
    }
    const string EMPTY_STR(m[0].size(), EMPTY);
    m.push_back(EMPTY_STR);
    m.insert(m.begin(), EMPTY_STR);

    ROWS = m.size();
    COLS = m[0].size();
    used.resize(ROWS, vector<int>(COLS));
    vector<vector<cell_borders>> empty_cb;
    empty_cb.resize(ROWS, vector<cell_borders>(COLS));

    ROWS -= 2;
    COLS -= 2;

    int id = 0;
    int result = 0;
    for (int row = 1; row <= ROWS; ++row) {
        for (int col = 1; col <= COLS; ++col) {
            char c = m[row][col];
            if (c != EMPTY && !used[row][col]) {
                S = 0;
                PP = 0;
                border_id = 0;
                cb = empty_cb;
                dfs(row, col, ++id);

                Dsu dsu(border_id);
                go(dsu, id);

                int sides = dsu.roots();

                result += sides * S;
            }
        }
    }

#if defined(TEST)
    EXPECT_EQ(result, 1206);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
