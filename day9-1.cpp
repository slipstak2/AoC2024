#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <cassert>
#include <set>
#include <chrono>

using namespace std;

//#define TEST
#define DAY 9

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

const int EMPTY = -1;


void output(const vector<int>& v) {
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] != EMPTY) {
            cout << v[i];
        }
        else {
            cout << ".";
        }
    }
    cout << endl;
}
int main()
{
    auto start = Now();

    FILE* f = freopen(InputFile().data(), "r", stdin);
    assert(f != nullptr);
    string row;
    cin >> row;
    vector<int> v;
    int totalCnt = 0;
    for (char c : row) {
        totalCnt += c - '0';
    }
    v.reserve(totalCnt);

    int id = 0;
    set<int> empty_pos;
    for (int i = 0; i < row.size(); ++i) {
        int cnt = row[i] - '0';
        if (i & 1) {
            while (cnt-- > 0) {
                v.push_back(EMPTY);
                empty_pos.insert(v.size() - 1);
            }
        }
        else {
            int curId = id++;
            while (cnt-- > 0) {
                v.push_back(curId);
            }
        }
    }

    assert(v.size() == totalCnt);

    for (int i = v.size() - 1; i >= 0; --i) {
        if (empty_pos.empty()) {
            break;
        }
        if (v[i] == EMPTY) {
            continue;
        }
        auto it_min_pos = empty_pos.begin();
        int pos = *it_min_pos;

        if (pos >= i) {
            break;
        }

        swap(v[i], v[pos]);
        empty_pos.erase(it_min_pos);
    }

    long long result = 0;
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] != EMPTY) {
            result += v[i] * (long long)i;
        }
    }
#if defined(TEST)
    assert(result == 1928);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
