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
#define DAY 16

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
vector<string> f;


struct point {
    int row = -1;
    int col = -1;
    point() = default;
    point(int row, int col) : row(row), col(col) {}
};

point operator + (const point& lhs, const point& rhs) {
    return point(lhs.row + rhs.row, lhs.col + rhs.col);
}

bool operator == (const point& lhs, const point& rhs) {
    return tie(lhs.row, lhs.col) == tie(rhs.row, rhs.col);
}

point UP(-1, 0);
point DOWN(1, 0);
point LEFT(0, -1);
point RIGHT(0, 1);

point delta[4] = {
    UP,
    LEFT,
    DOWN,
    RIGHT
};

bool operator < (const point& lhs, const point& rhs) {
    return 
        tie(lhs.row, lhs.col) < 
        tie(rhs.row, rhs.col);
}

struct vertex {
    point p;
    int dir = -1;
    vertex() = default;
    vertex(point p, int dir) : p(p.row, p.col), dir(dir) {}
};

bool operator < (const vertex& lhs, const vertex& rhs) {
    return 
        tie(lhs.p, lhs.dir) < 
        tie(rhs.p, rhs.dir);
}

point beg;
point fin;

map<vertex, int> V;
map<int, vertex> revV;

const INT MAX_VAL = 1e9;
struct Edge {
    int nxt_id;
    int value = 0;
};
vector<vector<Edge>> adj;
vector<INT> dist;
INT MIN_DIST = MAX_VAL;
vector<bool> marked;

bool dfs(int v_id) {
    vertex v = revV[v_id];
    bool isFind = false;
    if (v.p == point(11, 1)) {
        isFind = true;
    }
    if (v.p == fin) {
        return dist[v_id] == MIN_DIST;
    }

    bool isOK = false;
    for (const auto& edge : adj[v_id]) {
        if (dist[edge.nxt_id] == dist[v_id] + edge.value) {
            if (marked[edge.nxt_id]) {
                return true;
            }

            marked[edge.nxt_id] = true;
            if (dfs(edge.nxt_id)) {
                vertex nxt_v = revV[edge.nxt_id];
                f[nxt_v.p.row][nxt_v.p.col] = 'O';
                isOK = true;
            }
            else {
                marked[edge.nxt_id] = false;
            }

        }
    }
    return isOK;
}
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    std::string row;
    while (getline(cin, row)) {
        f.push_back(row);
    }
    ROWS = f.size();
    COLS = f[0].size();

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            point cur = point(row, col);
            char c = f[row][col];
            if (c == 'S') {
                beg = cur;
            }
            if (c == 'E') {
                fin = cur;
            }

            if (c != '#') {
                for (int dir = 0; dir < 4; ++dir) {
                    vertex v(cur, dir);
                    int id = V.size();
                    V[v] = id;
                    revV[id] = v;
                }
            }
        }
    }

    int V_SIZE = V.size();
    assert(V_SIZE % 4 == 0);
    adj.resize(V_SIZE);
    marked.resize(V_SIZE);

    for (const auto& [v, id] : V) {
        point pn = v.p + delta[v.dir];
        vertex vnxt(pn, v.dir);
        auto it = V.find(vnxt);
        if (it != V.end()) {
            int nid = it->second;
            adj[id].push_back(Edge(nid, 1));
        }

        {
            vertex vnxt(v.p, (v.dir + 1) % 4);
            int nid = V.at(vnxt);
            adj[id].push_back(Edge(nid, 1000));
        }

        {
            vertex vnxt(v.p, (v.dir - 1 + 4) % 4);
            int nid = V.at(vnxt);
            adj[id].push_back(Edge(nid, 1000));
        }
    }

    dist.resize(V_SIZE, MAX_VAL);
    vector<bool> used(V_SIZE);

    vertex v_beg(beg, 3); // RIGHT
    int id_beg = V.at(v_beg);
    dist[id_beg] = 0;

    for (;;) {
        int min_v = -1;
        for (int v = 0; v < V_SIZE; ++v) {
            if (!used[v] && dist[v] != MAX_VAL) {
                if (min_v == -1) {
                    min_v = v;
                }
                else {
                    if (dist[v] < dist[min_v]) {
                        min_v = v;
                    }
                }
            }
        }
        if (min_v == -1) {
            break;
        }
        used[min_v] = true;
        for (const auto& edge : adj[min_v]) {
            dist[edge.nxt_id] = min(dist[edge.nxt_id], dist[min_v] + edge.value);
        }
    }

    MIN_DIST = MAX_VAL;
    for (int dir = 0; dir < 4; ++dir) {
        vertex v_end(fin, dir);
        int id_end = V.at(v_end);
        MIN_DIST = min(MIN_DIST, dist[id_end]);
    }

    marked[id_beg] = true;
    dfs(id_beg);

    for (int row = 0; row < ROWS; ++row) {
        cout << f[row] << endl;
    }

    int result = 0;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (f[row][col] == 'O') {
                result++;
            }
        }
    }

#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 64);
#endif
    cout << result << endl;


    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
