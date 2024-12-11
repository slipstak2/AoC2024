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
#include <list>

using namespace std;

//#define TEST
#define DAY 11

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

void output(const list<long long>& data) {
    return;
    for (auto it = data.begin(); it != data.end(); ++it) {
        cout << *it << ' ';
    }
    cout << endl;
}

unordered_map<long long, unordered_map<long long, long long>> dp;

long long go(long long cur, int deep) {
    auto it = dp[cur].find(deep);
    if (it != dp[cur].end()) {
        return it->second;
    }

    long long& result = dp[cur][deep];
    if (deep == 0) {
        result = 1;
    }
    else {
        if (cur == 0) {
            long long nxt = 1;
            result = go(nxt, deep - 1);
        }
        else {
            std::string s = to_string(cur);
            if ((s.size() & 1) == 0) {
                std::string l = s.substr(0, s.size() / 2);
                std::string r = s.substr(s.size() / 2);
                result = go(stoll(l), deep - 1) + go(stoll(r), deep - 1);
            }
            else {
                long long nxt = cur * 2024;
                result = go(nxt, deep - 1);
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

    int steps = 75;
    long long result = 0;
    long long value;

    while (cin >> value) {
        result += go(value, steps);
    }

#if defined(TEST)
    EXPECT_EQ(result, 55312);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
