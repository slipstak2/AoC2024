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
#include <algorithm>

using namespace std;

// ad,ji,kp,ld,lj,lq,nb,tp,vd,vn,yz,zz
// ab,al,cq,cr,da,db,dr,fw,ly,mn,od,py,uh
//#define TEST
#define DAY 23

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
const int ALPHA_NUM = 'z' - 'a' + 1; // 26
const int MAX_VER = ALPHA_NUM * ALPHA_NUM - 1;

int convert(char x, char y) {
    return (x - 'a') * ALPHA_NUM + (y - 'a');
}
string revert(int code) {
    string s;
    s += (code / ALPHA_NUM + 'a');
    s += (code % ALPHA_NUM + 'a');
    return s;
}

vector<unordered_set<int>> adj;

struct clicka {
    clicka() = default;
    clicka(vector<int> v) : v(std::move(v)) {
        sort(v.begin(), v.end());
    }
    bool try_join(const clicka& other, clicka& res) const{
        for (int f : v) {
            for (int s : other.v) {
                if (f == s) {
                    continue;
                }
                if (!adj[f].count(s)) {
                    return false;
                }
            }
        }

        set<int> vs;
        for (int f : v) {
            vs.insert(f);
        }
        for (int s : other.v) {
            vs.insert(s);
        }

        vector<int> data(vs.begin(), vs.end());
        if (data == v) {
            return false;
        }
        else {
            res = clicka(data);
            return true;
        }

    }
    void output() const {
        cout << revert(v[0]);
        for (int i = 1; i < v.size(); ++i) {
            cout << "," << revert(v[i]);
        }
        cout << endl;
    }
    vector<int> v;
};

bool operator < (const clicka& lhs, const clicka& rhs) {
    return lhs.v < rhs.v;
}


int main()
{
    auto start = Now();

    FILE* file = freopen(InputFile().data(), "r", stdin);
    assert(file != nullptr);

    adj.resize(MAX_VER + 1);
    string s;
    char x[3], y[3];
    while (getline(cin, s)) {
        int x = convert(s[0], s[1]);
        string xx = revert(x);
        assert(revert(x)[0] == s[0]);
        assert(revert(x)[1] == s[1]);
        int y = convert(s[3], s[4]);
        string yy = revert(y);
        assert(revert(y)[0] == s[3]);
        assert(revert(y)[1] == s[4]);
        adj[x].insert(y);
        adj[y].insert(x);
    }
    int result = 0;

    int T = 't' - 'a';

    vector<clicka> clicks;
    for (int a = 0; a < MAX_VER; ++a) {
        if (a / ALPHA_NUM != T) {
            //continue;
        }
        for (int b : adj[a]) {
            if (b <= a) {
                continue;
            }
            for (int c : adj[b]) {
                if (c <= b) {
                    continue;
                }
                if (adj[c].count(a)) {
                    //if (a / ALPHA_NUM == T || b / ALPHA_NUM == T || c / ALPHA_NUM == T) {
                        vector<int> vs{ a, b, c };
                        clicks.push_back(clicka(vs));
                        result++;
                    //}
                    //cout << revert(a) << ", " << revert(b) << ", " << revert(c) << endl;
                }
            }
        }
    }

    set<clicka> prv_clicks;
    for (int i = 0; i < clicks.size(); ++i) {
        prv_clicks.insert(clicks[i]);
    }
    for (;;) {
        cout << prv_clicks.size() << endl;
        set<clicka> cur_clicks;
        for (const auto& prv_click : prv_clicks) {
            for (const auto& click3 : clicks) {
                clicka res;
                if (prv_click.try_join(click3, res)) {
                    cur_clicks.insert(res);
                }
            }
        }
        if (cur_clicks.empty()) {
            break;
        }
        prv_clicks.swap(cur_clicks);
    }

    assert(prv_clicks.size() == 1);
    prv_clicks.begin()->output();

#if defined(TEST)
    EXPECT_EQ<decltype(result)>(result, 7);
#endif
    cout << result << endl;

    auto diff = Now() - start;
    cout << TimeGetStr(diff.count());
    return 0;

}
