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
#include <algorithm>

using namespace std;

#define TEST
#define DAY 21

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

struct point {
    int row = -1;
    int col = -1;
    point(int row, int col) : row(row), col(col) {}
};

point operator + (const point& lhs, const point& rhs) {
    return point(lhs.row + rhs.row, lhs.col + rhs.col);
}

point delta[4] = {
    point(-1, 0),
    point(0, 1),
    point(1, 0),
    point(0, -1)
};

string deltaStr[4] = {
    "^",
    ">",
    "v",
    "<"
};


vector<string> adjDig[11][11];

vector<string> adjPult[256][256];

int keypadState = 10;
char robot1State = 'A';
char robot2State = 'A';

const int ROWSD = 4;
const int COLSD = 3;
char fieldDig[ROWSD][COLSD]{
    {7, 8, 9},
    {4, 5, 6},
    {1, 2, 3},
    {-1, 0, 10}
};
bool correctD(point p) {
    return 0 <= p.row && p.row < ROWSD && 0 <= p.col && p.col < COLSD;
}

int GETD(point p) {
    return fieldDig[p.row][p.col];
}

void bfsDig(point beg) {
    int begDigit = GETD(beg);
    adjDig[begDigit][begDigit].push_back("");
    vector<bool> used(11);
    vector<bool> was_in_queue(11);


    queue<point> q;
    was_in_queue[begDigit] = true;
    q.push(beg);
    while (!q.empty()) {
        point cur = q.front();
        int curDigit = GETD(cur);
        q.pop();
        for (int i = 0; i < 4; ++i) {
            point nxt = cur + delta[i];
            if (!correctD(nxt)) {
                continue;
            }
            int nxtDigit = GETD(nxt);
            if (nxtDigit == -1) {
                continue;
            }
            if (!used[nxtDigit]) {
                for (const string& way : adjDig[begDigit][curDigit]) {
                    adjDig[begDigit][nxtDigit].push_back(way + deltaStr[i]);
                }
                if (!was_in_queue[nxtDigit]) {
                    was_in_queue[nxtDigit] = true;
                    q.push(nxt);
                }
            }
        }
        used[curDigit] = true;
    }
}


const int ROWSP = 2;
const int COLSP = 3;
char fieldPult[ROWSP][COLSP]{
    { -1, '^', 'A'},
    {'<', 'v', '>'}
};
bool correctP(point p) {
    return 0 <= p.row && p.row < ROWSP && 0 <= p.col && p.col < COLSP;
}

int GETP(point p) {
    return fieldPult[p.row][p.col];
}

void bfsPult(point beg) {
    char begSymbol = GETP(beg);
    adjPult[begSymbol][begSymbol].push_back("");
    vector<bool> used(256);
    vector<bool> was_in_queue(256);


    queue<point> q;
    was_in_queue[begSymbol] = true;
    q.push(beg);
    while (!q.empty()) {
        point cur = q.front();
        char curSymbol = GETP(cur);
        q.pop();
        for (int i = 0; i < 4; ++i) {
            point nxt = cur + delta[i];
            if (!correctP(nxt)) {
                continue;
            }
            char nxtSymbol = GETP(nxt);
            if (nxtSymbol == -1) {
                continue;
            }
            if (!used[nxtSymbol]) {
                for (const string& way : adjPult[begSymbol][curSymbol]) {
                    adjPult[begSymbol][nxtSymbol].push_back(way + deltaStr[i]);
                }
                if (!was_in_queue[nxtSymbol]) {
                    was_in_queue[nxtSymbol] = true;
                    q.push(nxt);
                }
            }
        }
        used[curSymbol] = true;
    }
}

void precalc() {
    for (int row = 0; row < ROWSD; ++row) {
        for (int col = 0; col < COLSD; ++col) {
            if (fieldDig[row][col] == -1) {
                continue;
            }
            bfsDig(point(row, col));
        }
    }
    for (int row = 0; row < ROWSP; ++row) {
        for (int col = 0; col < COLSP; ++col) {
            if (fieldPult[row][col] == -1) {
                continue;
            }
            bfsPult(point(row, col));
        }
    }

}

const vector<string>& keypad(int curDigit) {
    auto& result = adjDig[keypadState][curDigit];
    keypadState = curDigit;
    return result;
}
string robot1(int c) {
    auto& keypadWays = keypad(c);
    char curState = robot1State;
    for (auto& keypadWay : keypadWays) {
        auto curWays = adjPult[curState]
        for (char c : keypadWay) {

        }
    }

    return "";
}
string robot2(int c) {
    robot1(c);

    return "";
}
string human(string code) {
    for (char c : code) {
        robot2(c == 'A' ? 10 : c - '0');
    }
    return "";
}
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    precalc();

    int result = 0;
    string s;
    while (getline(cin, s)) {
        string res = human(s);

    }


#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 0);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
