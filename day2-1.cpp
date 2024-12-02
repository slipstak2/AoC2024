#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;

//#define TEST

#if defined(TEST)
    const char* inputFile = "day2-test.txt";
#else
    const char* inputFile = "day2.txt";
#endif


bool check(const vector<int>& a) {
    int delta = a[1] - a[0];
    if (delta == 0) {
        return false;
    }
    bool isInc = delta > 0;
    for (int i = 1; i < a.size(); ++i) {
        int delta = a[i] - a[i - 1];
        if (delta == 0) {
            return false;
        }
        if (delta > 0 && !isInc) {
            return false;
        }
        if (delta < 0 && isInc) {
            return false;
        }
        if (abs(delta) > 3) {
            return false;
        }
    }
    return true;
}

void convert(vector<int>& a, const string row) {
    stringstream ss;
    ss << row;
    int value;
    a.clear();
    while (ss >> value) {
        a.push_back(value);
    }
}

int main()
{
    freopen(inputFile, "r", stdin);

    int result = 0;
    int value;
    vector<int> a;

    string row;
    while (getline(cin, row)) {
        convert(a, row);
        result += check(a);
    }

#if defined(TEST)
    assert(result == 2);
#endif
    cout << result << endl;
    return 0;

}
