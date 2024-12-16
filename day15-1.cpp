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
#define DAY 15

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

const char ROBOT = '@';
const char BOX = 'O';
const char BORDER = '#';
const char EMPTY = '.';


int ROWS = 0;
int COLS = 0;
vector<string> f;
vector<string> dirs;


struct point {
    int row = -1;
    int col = -1;
};
point operator + (const point& lhs, const point& rhs) {
    return { .row = lhs.row + rhs.row, .col = lhs.col + rhs.col };
}

unordered_map<char, point> delta = {
    {'<', {.row = 0,  .col = -1}},
    {'>', {.row = 0,  .col = 1}},
    {'^', {.row = -1, .col = 0}},
    {'v', {.row = 1,  .col = 0}}
};

struct Robot {
    point pos;
    void go(char m) {
        point d = delta.at(m);
        point nxt = pos + d;
        if (try_go(pos, nxt, d)) {
            swap(f[pos.row][pos.col], f[nxt.row][nxt.col]);
            pos = nxt;
        }
    }
    bool try_go(point prv, point cur, point d) {
        char c = f[cur.row][cur.col];
        if (c == BORDER) {
            return false;
        }
        if (c == EMPTY) {
            return true;
        }
        point nxt = cur + d;
        if (try_go(cur, nxt, d)) {
            swap(f[cur.row][cur.col], f[nxt.row][nxt.col]);
        }
    }
};

Robot robot;


void output() {
    return;
    static int step = 0;
    cout << step++ << endl;
    for (int row = 0; row < ROWS; ++row) {
        cout << f[row] << endl;
    }
    cout << endl;
}

void go() {
    output();
    for (const auto& dir : dirs) {
        for (const char d : dir) {
            robot.go(d);
            output();
        }
    }
}

int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    std::string row;
    while (getline(cin, row)) {
        if (row.empty()) {
            break;
        }
        f.push_back(row);
    }
    ROWS = f.size();
    COLS = f[0].size();
    while (getline(cin, row)) {
        dirs.push_back(row);
    }

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (f[row][col] == ROBOT) {
                robot.pos.row = row;
                robot.pos.col = col;
            }
        }
    }

    go();

    int result = 0;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (f[row][col] == BOX) {
                int cur = 100 * row + col;
                result += cur;
            }
        }
    }

#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 10092);
#endif
    cout << result << endl;


    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
