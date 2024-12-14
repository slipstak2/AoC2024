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
    void move() {
        row = (row + drow + ROWS) % ROWS;
        col = (col + dcol + COLS) % COLS;
    }
};

void go(robot& r) {
    for (int i = 0; i < 100; ++i) {
        r.row = (r.row + r.drow + ROWS) % ROWS;
        r.col = (r.col + r.dcol + COLS) % COLS;
    }
}

void output(vector<vector<int>>& f) {
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            cout << (f[row][col] ? 'X' : '.');
        }
        cout << endl;
    }
}
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);

    freopen("day14-tree.txt", "w", stdout);
    assert(file != nullptr);

    std::string row;
#if defined(TEST)
    COLS = 11;
    ROWS = 7;
#else 
    COLS = 101;
    ROWS = 103;
#endif

    vector<vector<int>> emptyF(ROWS, vector<int>(COLS));

    f = vector<vector<int>>(ROWS, vector<int>(COLS));
    vector<robot> robots;
    while (getline(cin, row)) {
        robot r;
        sscanf(row.data(), "p=%d,%d v=%d,%d", &r.col, &r.row, &r.dcol, &r.drow);
        robots.push_back(r);
    }

    for (int step = 0; step < 100000; ++step) {
        cout << step << endl;
        f = emptyF;
        for (const auto& r : robots) {
            f[r.row][r.col]++;
        }
        output(f);
        for (auto& r : robots) {
            r.move();
        }
    }

    int result = 0;
#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 12);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
