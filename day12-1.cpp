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

vector<string> m;
vector<vector<int>> used;

int delta_row[4] = {0, -1, 0, 1};
int delta_col[4] = {1, 0, -1, 0};

int P = 0;
int S = 0;

void dfs(int row, int col, int id) {
    used[row][col] = id;
    char c = m[row][col];
    S++;
    for (int i = 0; i < 4; ++i) {
        int nrow = row + delta_row[i];
        int ncol = col + delta_col[i];
        char nc = m[nrow][ncol];
        if (nc != c && used[nrow][ncol] != id) {
            P++;
        }
        if (nc == c && used[nrow][ncol] != id) {
            dfs(nrow, ncol, id);
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

    ROWS -= 2;
    COLS -= 2;

    int id = 0;
    int result = 0;
    for (int row = 1; row <= ROWS; ++row) {
        for (int col = 1; col <= COLS; ++col) {
            char c = m[row][col];
            if (c != EMPTY && !used[row][col]) {
                S = 0;
                P = 0;
                dfs(row, col, ++id);
                result += P * S;
            }
        }
    }

#if defined(TEST)
    EXPECT_EQ(result, 1930);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
