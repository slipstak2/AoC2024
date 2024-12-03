#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;

//#define TEST

#if defined(TEST)
const char* inputFile = "day3-test.txt";
#else
const char* inputFile = "day3.txt";
#endif

int main()
{
    freopen(inputFile, "r", stdin);
    string row;
    int result = 0;
    bool enable = true;
    while (getline(cin, row)) {
        const char* buffer = row.data();
        int x, y;
        char tmp[1024];
        for (int offset = 0; offset < row.size(); ++offset) {
            if (strstr(buffer + offset, "do()") == buffer + offset) {
                enable = true;
            }
            else if (strstr(buffer + offset, "don't()") == buffer + offset) {
                enable = false;
            }
            if (enable) {
                int cur = sscanf(buffer + offset, "mul(%d,%d", &x, &y);
                if (cur == 2) {
                    sprintf(tmp, "mul(%d,%d", x, y);
                    if (*(buffer + offset + strlen(tmp)) == ')') {
                        result += x * y;
                    }
                }
            }
        }
    }
#if defined(TEST)
    assert(result == 48);
#endif

    cout << result << endl;
    return 0;

}
