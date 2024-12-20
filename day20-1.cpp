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
#define DAY 20

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

const char BEG = 'S';
const char FIN = 'E';
const char EMPTY = '.';
const char BORDER = '#';

int ROWS;
int COLS;

struct point {
    int row = -1;
    int col = -1;
    point() = default;
    point(int row, int col) : row(row), col(col) {}
};

struct edge {
    point nxt;
    int value = -1;
    edge(point nxt, int value) : nxt(nxt.row, nxt.col), value(value) {}
};

bool operator == (const point& lhs, const point& rhs) {
    return tie(lhs.row, lhs.col) == tie(rhs.row, rhs.col);
}

bool operator < (const point & lhs, const point & rhs) {
    return tie(lhs.row, lhs.col) < tie(rhs.row, rhs.col);
}

bool operator != (const point& lhs, const point& rhs) {
    return !(lhs == rhs);
}

point operator + (const point& lhs, const point& rhs) {
    return point(lhs.row + rhs.row, lhs.col + rhs.col);
}

point operator - (const point& lhs, const point& rhs) {
    return point(lhs.row - rhs.row, lhs.col - rhs.col);
}



const point UP(-1, 0);
const point DOWN(1, 0);
const point LEFT(0, -1);
const point RIGHT(0, 1);

const point delta[4] = {
    DOWN,
    RIGHT,
    UP,
    LEFT
};

point beg;
point fin;

vector<string> f;
vector<vector<int>> distBEG;
vector<vector<int>> distFIN;

bool correct(point p) {
    return 0 <= p.row && p.row < ROWS && 0 <= p.col && p.col < COLS;
}

char GET(point p) {
    return f[p.row][p.col];
}

#define GETD(p) dist[p.row][p.col]
#define SETD(p, val) dist[p.row][p.col] = (val)

int GET_DIST_BEG(point p) {
    return distBEG[p.row][p.col];
}

int GET_DIST_FIN(point p) {
    return distFIN[p.row][p.col];
}

int bfs(point start, point finish, vector<vector<int>> &dist) {
    queue<point> q;
    dist.resize(ROWS, vector<int>(COLS, -1));

    SETD(start, 0);
    q.push(start);

    while (!q.empty()) {
        point cur = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            point nxt = cur + delta[i];
            if (GET(nxt) == EMPTY && GETD(nxt) == -1) {
                SETD(nxt, GETD(cur) + 1);
                q.push(nxt);
            }
        }
    }

    return GETD(finish);
}

map<point, vector<edge>> adj;

const int MAX_TELEPORT = 3;

void bfs2(point start) {
    queue<point> q;
    vector<vector<int>> dist(ROWS, vector<int>(COLS, -1));

    SETD(start, 0);
    q.push(start);

    while (!q.empty()) {
        point cur = q.front();
        q.pop();
        if (cur != start) {
            if (GET(cur) == EMPTY) {
                adj[start].push_back(edge(cur, GETD(cur)));
            }
        }
        for (int i = 0; i < 4; ++i) {
            point nxt = cur + delta[i];
            if (!correct(nxt)) {
                continue;
            }
            if (GETD(nxt) == -1) {
                int nxt_dist = GETD(cur) + (GET(nxt) == EMPTY ? 1 : 2);
                if (nxt_dist <= MAX_TELEPORT) {
                    SETD(nxt, nxt_dist);
                    q.push(nxt);
                }
            }
        }
    }
}

int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    string s;
    while (getline(cin, s)) {
        f.push_back(s);
    }
    ROWS = f.size();
    COLS = f[0].size();

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            char& c = f[row][col];
            if (c == BEG) {
                beg = point(row, col);
                c = EMPTY;
            }
            if (c == FIN) {
                fin = point(row, col);
                c = EMPTY;
            }
        }
    }

    int targetBeg = bfs(beg, fin, distBEG);
    int targetFin = bfs(fin, beg, distFIN);

    assert(targetBeg == targetFin);
    int target = targetBeg;

    for (int row = 1; row < ROWS - 1; ++row) {
        for (int col = 1; col < COLS - 1; ++col) {
            point cur(row, col);
            if (GET(cur) == EMPTY) {
                bfs2(cur);
            }
        }
    }

    map<int, int> res;
    for (int row = 1; row < ROWS - 1; ++row) {
        for (int col = 1; col < COLS - 1; ++col) {
            char c = f[row][col];
            if (c == BORDER) {
                point cur(row, col);
                for (int i = 0; i < 4; ++i) {
                    point nxt = cur + delta[i];
                    point prv = cur - delta[i];

                    if (GET(nxt) == EMPTY && GET(prv) == EMPTY) {
                        int x = GET_DIST_BEG(prv);
                        int y = GET_DIST_FIN(nxt);

                        int dist = x + y + 2;

                        res[target - dist]++;
                    }
                }
            }
        }
    }

    int result = 0;
    for (auto [pico, cnt] : res) {
        if (pico > 0) {
            //cout << cnt << " cheats save " << pico << " picoseconds" << endl;
        }
        if (pico >= 100) {
            result += cnt;
        }
    }



#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 0);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
