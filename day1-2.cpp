#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

//#define TEST

#if defined(TEST)
const char* inputFile = "day1-test.txt";
#else
const char* inputFile = "day1.txt";
#endif

int cnt(const vector<int>& v, int value) {
    int result = 0;
    for (int cur : v) {
        result += cur == value;
    }
    return result;
}

int main() {
    freopen(inputFile, "r", stdin);
    std::vector<int> x, y;
    int a, b;
    while (cin >> a >> b) {
        x.push_back(a);
        y.push_back(b);
    }
    long long result = 0;
    for (int a : x) {
        result += a * cnt(y, a);
    }
    cout << result << endl;
#if defined(TEST)
    assert(result == 31);
#endif
    return 0;
}