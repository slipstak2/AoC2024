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

#define TEST
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

set<int> empty_segments[10];

struct File {
    int offset = -1;
    int len = -1;
    void tryUpdate(int len, int offset) {
        if (this->offset == -1 || offset < this->offset) {
            this->len = len;
            this->offset = offset;
        }
    }
};

bool operator == (const File& lhs, const File& rhs) {
    return tie(lhs.offset, lhs.len) == tie(rhs.offset, rhs.len);
}

vector<File> fs;
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
            empty_segments[cnt].insert(v.size());
            while (cnt-- > 0) {
                v.push_back(EMPTY);
            }
        }
        else {
            int curId = id++;
            fs.push_back({ .offset = (int)v.size(), .len = cnt});
            while (cnt-- > 0) {
                v.push_back(curId);
            }
        }
    }

    assert(v.size() == totalCnt);

    for (int curId = fs.size() - 1; curId >= 0; --curId) {
        File& f = fs[curId];
        File res;
        for (int len = f.len; len < 10; ++len) {
            set<int>& offsets = empty_segments[len];
            if (offsets.empty()) {
                continue;
            }
            res.tryUpdate(len, *offsets.begin());
        }
        if (res.len == -1) {
            continue;
        }
        if (res.offset >= f.offset) {
            continue;
        }
        for (int i = 0; i < f.len; ++i) {
            swap(v[res.offset + i], v[f.offset + i]);
        }
        empty_segments[res.len].erase(res.offset);
        if (res.len - f.len != 0) {
            empty_segments[res.len - f.len].insert(res.offset + f.len);
        }
        //output(v);
    }


    long long result = 0;
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] != EMPTY) {
            result += v[i] * (long long)i;
        }
    }
#if defined(TEST)
    assert(result == 2858);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
