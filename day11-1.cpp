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

int main()
{
    auto start = Now();

    FILE* f = freopen(InputFile().data(), "r", stdin);
    assert(f != nullptr);
    string row;

    std::list<long long> data;
    long long value;
    while (cin >> value) {
        data.push_back(value);
    }
    
    int steps = 25;
    output(data);
    while (steps-- > 0) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (*it == 0) {
                *it = 1;
            }
            else {
                std::string cur = to_string(*it);
                if ((cur.size() & 1) == 0) {
                    std::string l = cur.substr(0, cur.size() / 2);
                    std::string r = cur.substr(cur.size() / 2);
                    *it = stoll(l);
                    data.insert(next(it), stoll(r));
                    ++it;
                }
                else {
                    *it *= 2024;
                }
            }
        }
        output(data);
    }

    int result = data.size();
#if defined(TEST)
    EXPECT_EQ(result, 55312);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
