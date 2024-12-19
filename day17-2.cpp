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
#include <map>
#include <chrono>
#include <functional>
#include <list>

using namespace std;

// 1,5,0,1,7,4,1,0,3
//#define TEST
#define DAY 17

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

INT RegA = -1;
INT RegB = -1;
INT RegC = -1;
string buffer;

int ReadRegister(char targetReg) {
    getline(cin, buffer);
    char reg;
    int value;
    int res = sscanf(buffer.data(), "Register %c: %d", &reg, &value);

    EXPECT_EQ(res, 2);
    EXPECT_EQ(reg, targetReg);
    return value;
}

vector<int> Split(string_view sv) {
    vector<int> result;
    while (!sv.empty()) {
        size_t pos = sv.find(',');
        int cur = stoi(string(
            pos == string_view::npos ?
            sv : sv.substr(0, pos)
        ));
        sv.remove_prefix(
            pos == string_view::npos ?
            sv.size() : pos + 1
        );
        result.push_back(cur);
    }
    return result;
}

vector<int> ReadProgram() {
    getline(cin, buffer);
    char commands[1024];
    int res = sscanf(buffer.data(), "Program: %s", commands);
    EXPECT_EQ(res, 1);
    return Split(commands);
}

INT COMB(int operand) {
    if (operand <= 3) {
        return operand;
    }
    if (operand == 4) {
        return RegA;
    }
    if (operand == 5) {
        return RegB;
    }
    if (operand == 6) {
        return RegC;
    }

    assert(false);
}

int POW2(INT value) {
    if (value > 31) {
        cerr << "overflow";
        exit(-1);
    }
    assert(value <= 31);
    return 1 << value;
}

// 0
void adv(int operand) {
    int DIV = POW2(COMB(operand));
    RegA = RegA / DIV;
}

// 1
void bxl(int operand) {
    RegB = RegB ^ operand;
}

// 2
void bst(int operand) {
    RegB = COMB(operand) % 8;
}

// 3
bool jnz(int operand) {
    return RegA != 0;
}

// 4
void bxc(int operand) {
    RegB = RegB ^ RegC;
}

// 5
int out(int operand) {
    return COMB(operand) % 8;
}

// 6
void bdv(int operand) {
    int DIV = POW2(COMB(operand));
    RegB = RegA / DIV;
}

// 7
void cdv(int operand) {
    int DIV = POW2(COMB(operand));
    RegC = RegA / DIV;
}

vector<int> commands;

int run() {
    for (int i = 0; i < commands.size(); i += 2) {
        int command = commands[i];
        int operand = commands[i + 1];
        if (command == 0) {
            adv(operand);
        }
        if (command == 1) {
            bxl(operand);
        }
        if (command == 2) {
            bst(operand);
        }
        if (command == 3) {
            if (jnz(operand)) {
                i = operand - 2;
            }
        }
        if (command == 4) {
            bxc(operand);
        }
        if (command == 5) {
            return out(operand);
        }
        if (command == 6) {
            bdv(operand);
        }
        if (command == 7) {
            cdv(operand);
        }
    }
    assert(false);
    return -1;
}


set<INT> ans;
void dfs(INT A, int pos) {
    if (pos == -1) {
        ans.insert(A);
        return;
    }
    for (int digit = 0; digit < 8; ++digit) {
        RegA = A * 8 + digit;
        if (run() == commands[pos]) {
            dfs(A * 8 + digit, pos - 1);
        }
    }
}
int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    RegA = ReadRegister('A');
    RegB = ReadRegister('B');
    RegC = ReadRegister('C');

    getline(cin, buffer);
    assert(buffer.empty());

    commands = ReadProgram();

    dfs(0, commands.size() - 1);

    INT result = *ans.begin();

#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 117440);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
