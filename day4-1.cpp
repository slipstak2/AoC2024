#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;

//#define TEST

#if defined(TEST)
const char* inputFile = "day4-test.txt";
#else
const char* inputFile = "day4.txt";
#endif

const string TARGET = "XMAS";
int ROWS = 0;
int COLS = 0;

inline bool correct(int row, int col) {
    return 0 <= row && row < ROWS && 0 <= col && col < COLS;
}
bool check(int pos, const vector<string>& f, int row, int col, int deltaRow, int deltaCol) {
    if (pos == TARGET.size()) {
        return true;
    }
    if (!correct(row, col)) {
        return false;
    }
    if (f[row][col] == TARGET[pos]) {
        return check(pos + 1, f, row + deltaRow, col + deltaCol, deltaRow, deltaCol);
    }
    return false;
}

int main()
{
    freopen(inputFile, "r", stdin);

    vector<string> f;

    string row;
    while (getline(cin, row)) {
        f.push_back(row);
    }

    ROWS = f.size();
    COLS = f[0].size();

    int result = 0;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            result += check(0, f, row, col, 1, 0);
            result += check(0, f, row, col, -1, 0);

            result += check(0, f, row, col, 0, 1);
            result += check(0, f, row, col, 0, -1);

            result += check(0, f, row, col, 1, 1);
            result += check(0, f, row, col, -1, -1);

            result += check(0, f, row, col, -1, 1);
            result += check(0, f, row, col, 1, -1);
        }
    }

#if defined(TEST)
    assert(result == 18);
#endif
    cout << result << endl;
    return 0;

}
