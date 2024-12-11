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
#define DAY 7

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

pair<INT, vector<INT>> convert(const std::string& row) {
    stringstream ss;
    ss << row;
    INT result;
    char c;
    ss >> result;
    ss >> c;
    INT value;
    vector<INT> data;
    while (ss >> value) {
        data.push_back(value);
    }
    return { result, data };
}

bool can(INT target, vector<INT>& data) {
    function<bool(INT cur, int pos)> go;

    go = [&target, &data, &go](INT cur, int pos) {
        if (pos == data.size()) {
            return cur == target;
        }
        return go(cur + data[pos], pos + 1) || go(cur * data[pos], pos + 1);
        };

    return go(data[0], 1);
}

int main()
{
    auto start = Now();

    FILE* f = freopen(InputFile().data(), "r", stdin);
    assert(f != nullptr);
    string row;

    INT result = 0;
    while (getline(cin, row)) {
        auto [target, data] = convert(row);
        if (can(target, data)) {
            result += target;
        }
    }

#if defined(TEST)
    EXPECT_EQ(result, 3749);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
