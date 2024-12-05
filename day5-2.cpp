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
const char* inputFile = "day5-test.txt";
#else
const char* inputFile = "day5.txt";
#endif


unordered_map<int, vector<int>> adj;
unordered_map<int, vector<int>> revAdj;

void correct(vector<int>& v) {
    for (int i = 0; i < v.size(); ++i) {
        int cur = v[i];
        vector<int>& a = adj[cur];
        for (int j = 0; j <  i; ++j) {
            if (find(a.begin(), a.end(), v[j]) != a.end()) {
                swap(v[i], v[j]);
                i = j;
                break;
            }
        }

    }
}

int main()
{
    freopen(inputFile, "r", stdin);

    int result = 0;
    string row;
    bool readAdj = true;
    while (getline(cin, row)) {
        if (row.empty()) {
            readAdj = false;
            continue;
        }
        if (readAdj) {
            int x, y;
            int num = sscanf(row.data(), "%d|%d", &x, &y);
            assert(num == 2);
            adj[x].push_back(y);
            revAdj[y].push_back(x);
        }
        else {
            string_view sv(row);
            unordered_set<int> black_list;
            vector<int> v;
            bool isOK = true;
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
                if (black_list.count(cur)) {
                    isOK = false;
                }
                v.push_back(cur);
                black_list.insert(revAdj[cur].begin(), revAdj[cur].end());
            }
            if (!isOK) {
                correct(v);
                result += v[v.size() / 2];
            }
        }
    }

#if defined(TEST)
    assert(result == 123);
#endif
    cout << result << endl;
    return 0;

}
