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
#include <map>
#include <chrono>
#include <functional>
#include <list>

using namespace std;

//#define TEST
#define DAY 18

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

#if defined(TEST)
const int ROWS = 7;
const int COLS = 7;
const int LIMIT = 12;
#else
const int ROWS = 71;
const int COLS = 71;
const int LIMIT = 1024;
#endif

const int BORDER = -1;
const int EMPTY = 0;

vector<vector<int>>f (ROWS, vector<int>(COLS, EMPTY));
vector<vector<int>>F(ROWS, vector<int>(COLS, EMPTY));

struct point {
    int row = -1;
    int col = -1;
};

point operator + (const point& lhs, const point& rhs) {
    return { .row = lhs.row + rhs.row, .col = lhs.col + rhs.col };
}

point delta[4] = {
    point(0, 1),
    point(0, -1),
    point(-1, 0),
    point(1, 0)
};

bool correct(point p) {
    return 0 <= p.row && p.row < ROWS && 0 <= p.col && p.col < COLS;
}
int GET(point p) {
    if (correct(p)) {
        return f[p.row][p.col];
    }
    return BORDER;
}

void SET(point p, int val) {
    f[p.row][p.col] = val;
}


int bfs() {
    point beg(0, 0);
    point end = point(ROWS - 1, COLS - 1);

    queue<point> q;
    q.push(beg);
    f[beg.row][beg.col] = 1;

    while (!q.empty()) {
        point cur = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            point nxt = cur + delta[i];
            if (GET(nxt) == EMPTY) {
                SET(nxt, GET(cur) + 1);
                q.push(nxt);
            }
        }
    }
    int result = f[end.row][end.col];
    if (result == EMPTY) {
        return EMPTY;
    }
    return result - 1;
}
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);


    int row, col;
    std::string s;
    
    string result;
    for (;;) {
        if (!getline(cin, s)) {
            break;
        }
        sscanf(s.data(), "%d,%d", &col, &row);
        F[row][col] = BORDER;
        f = F;
        if (bfs() == EMPTY) {
            result = s;
            break;
        }

    }

#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, "6,1");
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
