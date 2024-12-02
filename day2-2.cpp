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


bool check_base(const vector<int>& a) {
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

bool check(const vector<int>& a) {
    if (check_base(a)) {
        return true;
    }
    for (int i = 0; i < a.size(); ++i) {
        vector<int> v;
        v.reserve(a.size() - 1);
        for (int j = 0; j < a.size(); ++j) {
            if (j != i) {
                v.push_back(a[j]);
            }
        }
        if (check_base(v)) {
            return true;
        }
    }
    return false;
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

    cout << result << endl;
#if defined(TEST)
    assert(result == 4);
#endif
    return 0;

}
