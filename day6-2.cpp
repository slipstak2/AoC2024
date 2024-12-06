#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <cassert>

using namespace std;

//#define TEST

#if defined(TEST)
const char* inputFile = "day6-test.txt";
#else
const char* inputFile = "day6.txt";
#endif


const char BORDER = '#';
const char EMPTY = '.';
const char OUT = '*';
const char USED = 'X';

std::string directions = "^>v<";
int delta_row[4] = { -1, 0, 1, 0 };
int delta_col[4] = { 0, 1, 0, -1 };
int pos = -1;

vector<string> f;
int ROWS = -1;
int COLS = -1;

bool correct(int row, int col) {
    return 0 <= row && row < ROWS && 0 <= col && col < COLS;
}
char nxt(int row, int col) {
    int nrow = row + delta_row[pos];
    int ncol = col + delta_col[pos];
    if (!correct(nrow, ncol)) {
        return OUT;
    }
    return f[nrow][ncol];
}
bool go(int row, int col) {
    int result = 0;
    int iterations = ROWS * COLS + 1;
    while (iterations) {
        if (f[row][col] == EMPTY) {
            f[row][col] = USED;
            result++;
        }
        else {
            char next_symbol = nxt(row, col);
            if (next_symbol == BORDER) {
                pos = (pos + 1) % 4;
            }
            else if (next_symbol == EMPTY || next_symbol == USED) {
                row += delta_row[pos];
                col += delta_col[pos];
                iterations--;
            }
            else if (next_symbol == OUT) {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    freopen(inputFile, "r", stdin);

    string line;

    int begRow = -1;
    int begCol = -1;
    while (getline(cin, line)) {
        assert(!line.empty());
        f.push_back(line);
        for (int col = 0; col < line.size(); ++col) {
            if (line[col] != BORDER && line[col] != EMPTY) {
                assert(begRow == -1);
                begRow = f.size() - 1;
                begCol = col;
            }
        }
    }
    ROWS = f.size();
    COLS = f[0].size();
    pos = directions.find(f[begRow][begCol]);
    assert(pos != -1);

    int result = 0;
    f[begRow][begCol] = EMPTY;
    vector<string> ff;
    int begPos = pos;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (row == begRow && col == begCol) {
                continue;
            }
            if (f[row][col] == EMPTY) {
                ff = f;
                f[row][col] = BORDER;
                pos = begPos;
                result += go(begRow, begCol);
                f = ff;
            }
        }
    }

#if defined(TEST)
    assert(result == 6);
#endif
    cout << result << endl;
    return 0;

}
