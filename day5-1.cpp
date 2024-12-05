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
            adj[y].push_back(x);
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
                    break;
                }
                v.push_back(cur);
                black_list.insert(adj[cur].begin(), adj[cur].end());
            }
            if (isOK) {
                result += v[v.size() / 2];
            }
        }
    }

#if defined(TEST)
    assert(result == 143);
#endif
    cout << result << endl;
    return 0;

}
