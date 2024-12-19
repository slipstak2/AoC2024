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
#define DAY 19

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

vector<string> Split(string_view sv) {
    vector<string> result;
    while (!sv.empty()) {
        size_t pos = sv.find(',');
        string cur = string(
            pos == string_view::npos ?
            sv : sv.substr(0, pos)
        );
        sv.remove_prefix(
            pos == string_view::npos ?
            sv.size() : pos + 2
        );
        result.push_back(cur);
    }
    return result;
}

bool is_substring(const string& text, int offset, const string& s) {
    int i = 0;
    for (i = 0; i < s.size() && offset + i < text.size(); ++i) {
        if (text[offset + i] != s[i]) {
            return false;
        }
    }
    return i == s.size();
}

vector<string> words;

INT check(const string& s) {
    vector<INT> cnt(s.size());
    cnt[0] = 1;
    INT result = 0;
    for (int pos = 0; pos < s.size(); ++pos) {
        if (cnt[pos]) {
            for (const string& word : words) {
                if (is_substring(s, pos, word)) {
                    int nxt = pos + word.size();
                    INT& res = nxt == s.size() ? result : cnt[nxt];
                    res += cnt[pos];
                }
            }
        }
    }
    return result;
}
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    string s;
    getline(cin, s);
    words = Split(s);
    for (auto& word : words) {
        reverse(word.begin(), word.end());
    }
    getline(cin, s);
    assert(s.empty());

    INT result = 0;
    while (getline(cin, s)) {
        reverse(s.begin(), s.end());
        result += check(s);
    }


#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 16);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
