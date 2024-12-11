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
#define DAY 8

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
const int MAX_VERTEX = 256;
const char EMPTY = '.';
const char ANTI = '#';

struct point {
    int row;
    int col;
};

bool correct(int row, int col) {
    return 0 <= row && row < ROWS && 0 <= col && col < COLS;
}

int try_add(int& result, vector<string>& anti, int row, int col) {
    if (!correct(row, col)) {
        return -1;
    }
    char& c = anti[row][col];
    if (c == EMPTY) {
        c = ANTI;
        result++;
        return 1;
    }
    return 0;
}
int run(const vector<string>& m) {
    vector<point> coords[MAX_VERTEX];
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            char c = m[row][col];
            if (c != EMPTY) {
                coords[c].push_back({ .row = row, .col = col });
            }
        }
    }

    vector<string> anti(ROWS, string(COLS, EMPTY));

    int result = 0;
    for (int c = 0; c < MAX_VERTEX; ++c) {
        if (coords[c].size() <= 1) {
            continue;
        }
        const vector<point>& v = coords[c];
        for (int i = 0; i < v.size(); ++i) {
            for (int j = i + 1; j < v.size(); ++j) {
                const point& l = v[i];
                const point& r = v[j];
                try_add(result, anti, l.row, l.col);
                try_add(result, anti, r.row, r.col);
                int drow = r.row - l.row;
                int dcol = r.col - l.col;

                for (int mul = 1; ; ++mul) {
                    if (try_add(result, anti, l.row - mul * drow, l.col - mul * dcol) == -1) {
                        break;
                    }
                 }

                for (int mul = 1; ; ++mul) {
                    if (try_add(result, anti, r.row + mul * drow, r.col + mul * dcol) == -1) {
                        break;
                    }
                }
            }
        }
    }
    return result;

}
int main()
{
    auto start = Now();

    FILE* f = freopen(InputFile().data(), "r", stdin);
    assert(f != nullptr);
    string row;

    vector<string> m;
    while (getline(cin, row)) {
        m.push_back(row);
    }
    ROWS = m.size();
    COLS = m[0].size();

    int result = run(m);

#if defined(TEST)
    EXPECT_EQ(result, 34);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
