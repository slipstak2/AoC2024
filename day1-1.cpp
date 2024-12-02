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

int main() {
    freopen(inputFile, "r", stdin);
    std::vector<int> x, y;
    int a, b;
    while (cin >> a >> b) {
        x.push_back(a);
        y.push_back(b);
    }
    sort(x.begin(), x.end());
    sort(y.begin(), y.end());
    long long result = 0;
    for (int i = 0; i < x.size(); ++i) {
        result += abs(x[i] - y[i]);
    }

    cout << result << endl;
#if defined(TEST)
    assert(result == 11);
#endif
    return 0;
}