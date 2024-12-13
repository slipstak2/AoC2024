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

template<typename T>
void EXPECT_EQ(T lhs, T rhs) {
    if (lhs != rhs) {
        cerr << lhs << " != " << rhs << endl;
        exit(-1);
    }
}

const INT DELTA = 10000000000000;

struct Button {
    INT x = 0;
    INT y = 0;
};

bool operator == (const Button& lhs, const Button& rhs) {
    return tie(lhs.x, lhs.y) == tie(rhs.x, rhs.y);
}
Button operator * (const Button& lhs, INT cnt) {
    return { .x = lhs.x * cnt, .y = lhs.y * cnt };
}

INT DET(Button A, Button B) {
    return A.x * B.y - A.y * B.x;
}
INT MAX_VAL = 1e18;

INT tryOne(Button A, Button T) {
    if (A == T) {
        return 0;
    }
    INT cnt = T.x / A.x;
    A = A * cnt;

    return A == T ? cnt : MAX_VAL;
}
INT calc(Button A, Button B, Button T) {

    INT det = DET(A, B);
    if (det != 0) {
        INT detA = DET(T, B);
        INT detB = DET(A, T);
        INT mulA = detA / det;
        INT mulB = detB / det;
        if (mulA * A.x + mulB * B.x == T.x) {
            if (mulA * A.y + mulB * B.y == T.y) {
                return 3 * mulA + mulB;
            }
        }
    } else{
        INT res = tryOne(A, T);
        res = min(res, tryOne(B, T));
        return res == MAX_VAL ? 0 : res;
    }

    return 0;

}
int main()
{
    auto start = Now();

    FILE* f = freopen(InputFile().data(), "r", stdin);
    assert(f != nullptr);
    string row;

    INT result = 0;
    for (;;) {
        if (!getline(cin, row)) {
            break;
        }
        Button A, B, T;
        sscanf(row.data(), "Button A: X+%lld, Y+%lld", &A.x, &A.y);
        getline(cin, row);
        sscanf(row.data(), "Button B: X+%lld, Y+%lld", &B.x, &B.y);
        getline(cin, row);
        int res = sscanf(row.data(), "Prize: X=%lld, Y=%lld", &T.x, &T.y);
        T.x += DELTA;
        T.y += DELTA;
        result += calc(A, B, T);

        getline(cin, row);
    }

#if defined(TEST)
    EXPECT_EQ<INT>(result, 875318608908);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
