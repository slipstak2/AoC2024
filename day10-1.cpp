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

using namespace std;

//#define TEST
#define DAY 10

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

vector<string> m;

struct point {
    int row;
    int col;
};

int deltaRow[4] = {-1,0,1,0};
int deltaCol[4] = { 0,-1,0,1};

bool correct(int row, int col) {
    return 0 <= row && row < ROWS && 0 <= col && col < COLS;
}

int run(int row, int col) {
    int result = 0;
    vector<vector<bool>> used(ROWS, vector<bool>(COLS));
    queue<point> q;

    used[row][col] = true;
    q.push({ .row = row, .col = col });

    while (!q.empty()) {
        point cur = q.front();
        q.pop();
        for (int k = 0; k < 4; ++k) {
            int nrow = cur.row + deltaRow[k];
            int ncol = cur.col + deltaCol[k];
            if (correct(nrow, ncol) && !used[nrow][ncol] && m[nrow][ncol] == m[cur.row][cur.col] + 1) {
                used[nrow][ncol] = true;
                if (m[nrow][ncol] == '9') {
                    result++;
                }
                else {
                    q.push({ .row = nrow, .col = ncol });
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

    while (getline(cin, row)) {
        m.push_back(row);
    }

    ROWS = m.size();
    COLS = m[0].size();

    int result = 0;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (m[row][col] == '0') {
                result += run(row, col);
            }
        }
    }
    
#if defined(TEST)
    EXPECT_EQ(result, 36);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
