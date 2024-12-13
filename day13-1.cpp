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
#define DAY 13

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

struct Button {
    int x = 0;
    int y = 0;
};

const int MAX_VAL = 1e9;
int calc(Button A, Button B, Button T) {
    int res = MAX_VAL;
    for (int mulA = 0; mulA <= 1000; ++mulA) {
        for (int mulB = 0; mulB <= 1000; ++mulB) {
            if (mulA * A.x + mulB * B.x == T.x) {
                if (mulA * A.y + mulB * B.y == T.y) {
                    res = min(res, 3 * mulA + mulB);
                }
            }
        }
    }
    return res == MAX_VAL ? 0 : res;
}
int main()
{
    auto start = Now();

    FILE* f = freopen(InputFile().data(), "r", stdin);
    assert(f != nullptr);
    string row;

    int result = 0;
    for (;;) {
        if (!getline(cin, row)) {
            break;
        }
        Button A, B, T;
        sscanf(row.data(), "Button A: X+%d, Y+%d", &A.x, &A.y);
        getline(cin, row);
        sscanf(row.data(), "Button B: X+%d, Y+%d", &B.x, &B.y);
        getline(cin, row);
        int res = sscanf(row.data(), "Prize: X=%d, Y=%d", &T.x, &T.y);
        result += calc(A, B, T);

        getline(cin, row);
    }

#if defined(TEST)
    EXPECT_EQ(result, 480);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
