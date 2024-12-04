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

int ROWS = 0;
int COLS = 0;
vector<string> f;

inline bool correct(int row, int col) {
    return 0 <= row && row < ROWS && 0 <= col && col < COLS;
}

int get(int row, int col) {
    if (!correct(row, col)) {
        return 4;
    }
    if (f[row][col] == 'M') {
        return 1;
    }
    if (f[row][col] == 'S') {
        return 2;
    }
    return 4;
}
bool check(int row, int col) {
    int x = get(row + 1, col + 1) + get(row - 1, col - 1);
    int y = get(row + 1, col - 1) + get(row - 1, col + 1);

    return x == y && x == 3;
}
int main()
{
    freopen(inputFile, "r", stdin);

    string row;
    while (getline(cin, row)) {
        f.push_back(row);
    }

    ROWS = f.size();
    COLS = f[0].size();

    int result = 0;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (f[row][col] == 'A') {
                result += check(row, col);
            }

        }
    }

#if defined(TEST)
    assert(result == 9);
#endif
    cout << result << endl;
    return 0;

}
