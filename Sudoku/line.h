#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <vector>
using namespace std;

#ifndef const_N_DEFINED
#define const_N_DEFINED
const int N = 9;
#endif // const_N_DEFINED

struct Line {
    bool used[N + 1];
    bool cell[N + 1];

    void init() {
        for (int i = 0; i < N; i++) used[i] = false;
        for (int i = 0; i < N; i++) cell[i] = false;
    }

    Line() {
        init();
    }

    vector<int> getEmptyCells() {
        vector<int> res;
        for (int i = 0; i < N; i++) {
            if (!cell[i]) {
                res.push_back(i);
            }
        }
        return res;
    }

    void setCell(int x, int number) {
        cell[x] = used[number] = true;
    }

    void resetCell(int x, int number) {
        cell[x] = used[number] = false;
    }

    bool isUsed(int x) {
        return used[x];
    }
};

#endif // LINE_H_INCLUDED
