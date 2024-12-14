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
#define DAY 14

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

template<typename T>
void EXPECT_EQ(T lhs, T rhs) {
    if (lhs != rhs) {
        cerr << lhs << " != " << rhs << endl;
        exit(-1);
    }
}

int ROWS;
int COLS;

vector<vector<int>> f;

struct robot {
    int row = 0;
    int col = 0;
    int drow = 0;
    int dcol = 0;
};

void go(robot& r) {
    for (int i = 0; i < 100; ++i) {
        r.row = (r.row + r.drow + ROWS) % ROWS;
        r.col = (r.col + r.dcol + COLS) % COLS;
    }
}

long long calc(int dx, int dy) {
    int HALF_ROWS = ROWS / 2;
    int HALF_COLS = COLS / 2;
    long long cnt = 0;
    for (int row = dx * HALF_ROWS; row <= (dx + 1) * HALF_ROWS; ++row) {
        for (int col = dy * HALF_COLS; col <= (dy + 1) * HALF_COLS; ++col) {
            if (row == HALF_ROWS) {
                continue;
            }
            if (col == HALF_COLS) {
                continue;
            }
            cnt += f[row][col];
        }
    }
    return cnt;
}
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    std::string row;
#if defined(TEST)
    COLS = 11;
    ROWS = 7;
#else 
    COLS = 101;
    ROWS = 103;
#endif

    f = vector<vector<int>>(ROWS, vector<int>(COLS));
    while (getline(cin, row)) {
        robot r;
        // p=3,0 v=-2,-2
        sscanf(row.data(), "p=%d,%d v=%d,%d", &r.col, &r.row, &r.dcol, &r.drow);
        go(r);
        f[r.row][r.col]++;
    }

    long long result = calc(0, 0) * calc(0, 1) * calc(1, 0) * calc(1, 1);

#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 12);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
