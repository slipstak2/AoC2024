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

//#define TEST
#define DAY 23

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
const int ALPHA_NUM = 'z' - 'a' + 1; // 26
const int MAX_VER = ALPHA_NUM * ALPHA_NUM - 1;

int convert(char x, char y) {
    return (x - 'a') * ALPHA_NUM + (y - 'a');
}
string revert(int code) {
    string s;
    s += (code / ALPHA_NUM + 'a');
    s += (code % ALPHA_NUM + 'a');
    return s;
}

vector<unordered_set<int>> adj;
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    adj.resize(MAX_VER + 1);
    string s;
    char x[3], y[3];
    while (getline(cin, s)) {
        int x = convert(s[0], s[1]);
        string xx = revert(x);
        assert(revert(x)[0] == s[0]);
        assert(revert(x)[1] == s[1]);
        int y = convert(s[3], s[4]);
        string yy = revert(y);
        assert(revert(y)[0] == s[3]);
        assert(revert(y)[1] == s[4]);
        adj[x].insert(y);
        adj[y].insert(x);
    }
    int result = 0;

    int T = 't' - 'a';
    for (int a = 0; a < MAX_VER; ++a) {
        if (a / ALPHA_NUM != T) {
            //continue;
        }
        for (int b : adj[a]) {
            if (b <= a) {
                continue;
            }
            for (int c : adj[b]) {
                if (c <= b) {
                    continue;
                }
                if (adj[c].count(a)) {
                    if (a / ALPHA_NUM == T || b / ALPHA_NUM == T || c / ALPHA_NUM == T) {
                        result++;
                    }
                    //cout << revert(a) << ", " << revert(b) << ", " << revert(c) << endl;
                }
            }
        }
    }


#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 7);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
