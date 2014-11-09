#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED
#include <vector>
using namespace std;

#define pii pair<int, int>
#ifndef const_N_DEFINED
#define const_N_DEFINED
const int N = 9;
#endif // const_N_DEFINED

struct Square {

    bool used[N + 1];
    bool cell[3][3];

    void init() {
        for (int i = 0; i < N; i++) used[i] = false;
        for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) cell[i][j] = false;
    }

    Square() {
        init();
    }

    vector<pii> getEmptyCells() {
        vector<pii> res;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (!cell[i][j]) {
                    res.push_back(make_pair(i, j));
                }
            }
        }
        return res;
    }

    void setCell(int x, int y, int number) {
        cell[x % 3][y % 3] = used[number] = true;
    }

    void resetCell(int x, int y, int number) {
        cell[x % 3][y % 3] = used[number] = false;
    }

    bool isUsed(int x) {
        return used[x];
    }
};

#endif // SQUARE_H_INCLUDED
