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
    point left() {
        return { .row = row, .col = col - 1 };
    }
    point right() {
        return { .row = row, .col = col + 1 };
    }
};
point operator + (const point& lhs, const point& rhs) {
    return { .row = lhs.row + rhs.row, .col = lhs.col + rhs.col };
}
point operator - (const point& lhs, const point& rhs) {
    return { .row = lhs.row - rhs.row, .col = lhs.col - rhs.col };
}
bool operator == (const point& lhs, const point& rhs) {
    return tie(lhs.row, lhs.col) == tie(rhs.row, rhs.col);
}

point RIGHT{ .row = 0, .col = 1 };
point LEFT{ .row = 0,  .col = -1 };
point UP{ .row = -1,. col = 0 };
point DOWN{ .row = 1, .col = 0 };

unordered_map<char, point> delta = {
    {'<', LEFT},
    {'>', RIGHT},
    {'^', UP},
    {'v', DOWN}
};

inline void SWAP(point x, point y) {
    swap(f[x.row][x.col], f[y.row][y.col]);
}

inline char GET(point p) {
    return f[p.row][p.col];
}


struct Robot {
    point pos;
    bool check_vertical(point cur, point d) {
        assert(GET(cur) == '[');
        point nxt = cur + d;
        if (GET(nxt) == BORDER || GET(nxt.right()) == BORDER) {
            return false;
        }
        if (GET(nxt) == EMPTY && GET(nxt.right()) == EMPTY) {
            return true;
        }
        if (GET(nxt) == '[') {
            return check_vertical(nxt, d);
        }

        point nxt_l = nxt.left();
        point nxt_r = nxt.right();

        bool result = true;
        if (GET(nxt_l) == '[') {
            result &= check_vertical(nxt_l, d);
        }
        if (GET(nxt_r) == '[') {
            result &= check_vertical(nxt_r, d);
        }
        return result;
    }
    void move_vertical(point cur, point d) {
        point nxt = cur + d;
        if (GET(nxt) == '[') {
            move_vertical(nxt, d);
        }
        point nxt_l = nxt.left();
        point nxt_r = nxt.right();

        if (GET(nxt_l) == '[') {
            move_vertical(nxt_l, d);
        }
        if (GET(nxt_r) == '[') {
            move_vertical(nxt_r, d);
        }

        assert(GET(nxt) == EMPTY && GET(nxt.right()) == EMPTY);
        SWAP(cur, nxt);
        SWAP(cur.right(), nxt.right());
    }
    bool try_go_vertical(point cur, point d) {
        point nxt = cur + d;
        if (GET(nxt) == BORDER) {
            return false;
        }
        if (GET(nxt) == EMPTY) {
            return true;
        }
        else {
            point nxt = cur + d;
            point check_next;

            if (GET(nxt) == '[') {
                check_next = nxt;
            }
            else if (GET(nxt) == ']') {
                check_next = nxt.left();
            }
            else {
                assert(false);
            }
            if (check_vertical(check_next, d)) {
                move_vertical(check_next, d);
                return true;
            }
            return false;
        }
    }
    point try_go_horizontal(point base, point nxt, point d) {
        if (GET(nxt) == BORDER) {
            return base;
        }
        if (GET(nxt) == EMPTY) {
            return nxt;
        }

        if (GET(nxt) == '[' && d == RIGHT) {
            return try_go_horizontal(base, nxt + d + d, d);
        }
        if (GET(nxt) == ']' && d == LEFT) {
            return try_go_horizontal(base, nxt + d + d, d);
        }

        return base;
    }
    void go(char m) {
        point d = delta.at(m);
        point nxt = pos + d;
        if (d == UP || d == DOWN) {
            if (try_go_vertical(pos, d)) {
                SWAP(pos, nxt);
                pos = nxt;
            }

        }
        else {
            point last = try_go_horizontal(pos, nxt, d);
            if (last != pos) {
                for (point p = last; p != pos; p = p - d) {
                    SWAP(p, p - d);
                }
                pos = nxt;
            }
        }
    }
};

Robot robot;


void output(char c = 0) {
    return;
    static int step = 0;
    cout << step++ << (c == 0 ? ' ' : c) << endl;
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
            output(d);
        }
    }
}

string convert(const std::string& s) {
    string target;
    target.resize(s.size() * 2);
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '#') {
            target[2 * i] = target[2 * i + 1] = '#';
        }
        else if (s[i] == 'O') {
            target[2 * i] = '[';
            target[2 * i + 1] = ']';
        }
        else if (s[i] == '.') {
            target[2 * i] = target[2 * i + 1] = '.';
        }
        else if (s[i] == '@') {
            target[2 * i] = '@';
            target[2 * i + 1] = '.';
        }
    }
    return target;
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
        f.push_back(convert(row));
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
            if (f[row][col] == '[') {
                int cur = 100 * row + col;
                result += cur;
            }
        }
    }

#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 9021);
#endif
    cout << result << endl;


    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
