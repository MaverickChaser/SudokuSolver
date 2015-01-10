#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "square.h"
#include "line.h"

#define RESET_COLOR "\e[m"
#define MAKE_GREEN "\e[32m"
#define fi first
#define se second
#define mp make_pair

#define FILENAME "input"

using namespace std;

const int MAX_DEPTH = 3;

int Board[N][N];
int emptyCellsNumber;

int getSquareId(int, int);
bool UpdateAll(int depth, int entryPoint);
bool UpdateRows(vector<pair<pii, int> >&, int);
bool UpdateCols(vector<pair<pii, int> >&, int);
bool UpdateSquares(vector<pair<pii, int> >&, int);

void printBoard(pii cell = make_pair(-1, -1)) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!Board[i][j]) {
                cout << '.';
            } else {
                cout << Board[i][j];
            }
        }
        cout << "\n";
    }
    cout << "<--------------------->\n";
}

inline int getSquareId(pii cell) {
    int k1 = cell.first / 3;
    int k2 = cell.second / 3;
    return 3 * k1 + k2;
}

inline int getSquareId(int x, int y) {
    int k1 = x / 3;
    int k2 = y / 3;
    return 3 * k1 + k2;
}


inline pii getCell(int id, pii cell) {
    int k1 = id / 3;
    int k2 = id % 3;
    return make_pair(cell.first + 3 * k1, cell.second + 3 * k2);
}

pii getCell(int id, int x, int y) {
    int k1 = id / 3;
    int k2 = id % 3;
    return make_pair(x + 3 * k1, y + 3 * k2);
}

template<typename T> vector<int> getLeftNumbers(T t) {
    vector<int> res;
    for (int i = 1; i <= N; i++) {
        if (!t.isUsed(i)) {
            res.push_back(i);
        }
    }
    return res;
}

bool Can(int num, Line row, Line col, Square s) {
    return !row.isUsed(num) && !col.isUsed(num) && !s.isUsed(num);
}

Line rows[N + 1], cols[N + 1];
Square squares[N + 1];


int ReadInput(Line rows[], Line cols[], Square squares[]) {
    freopen(FILENAME, "r", stdin);
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char c;
            cin >> c;
            Board[i][j] = (c != '.' ? c - '0' : 0);
            if (Board[i][j] == 0) {
                cnt++;
            } else {
                rows[i].setCell(j, Board[i][j]);
                cols[j].setCell(i, Board[i][j]);
                squares[getSquareId(i, j)].setCell(i, j, Board[i][j]);
            }
        }
    }
    return cnt;
}

void setCell(pii curCell, int curNumber) {
    Board[curCell.fi][curCell.se] = curNumber;
    rows[curCell.fi].setCell(curCell.se, curNumber);
    cols[curCell.se].setCell(curCell.fi, curNumber);
    squares[getSquareId(curCell.first, curCell.second)].setCell(curCell.fi, curCell.se, curNumber);
    emptyCellsNumber--;
}

void resetCell(pii curCell, int curNumber) {
    Board[curCell.fi][curCell.se] = 0;
    rows[curCell.fi].resetCell(curCell.se, curNumber);
    cols[curCell.se].resetCell(curCell.fi, curNumber);
    squares[getSquareId(curCell.first, curCell.second)].resetCell(curCell.fi, curCell.se, curNumber);
    emptyCellsNumber++;
}

ostream& operator << (ostream & out, const pii p) {
	out << "(" << p.first << "," << p.second << ")";
	return out;
}

pair<pii, bool> ChooseCell(vector<pii> & Cells, int curNumber, int depth) {
    int amo = Cells.size();

	if (amo == 1) {
		setCell(Cells.back(), curNumber);
		return mp(Cells.back(), 1);
	}

    if (depth && amo <= 2) {
        int cnt = 0;
        pii curCell = mp(-1, -1);

        for (int k = 0; k < amo && !cnt; k++) {
            setCell(Cells[k], curNumber);

			bool result = UpdateAll(depth - 1, 1);
            resetCell(Cells[k], curNumber);
            if (result) {
                cnt++;
                curCell = Cells[k];
            }
        }

        if (cnt) {
            // Try guessing
            setCell(curCell, curNumber);
            //printBoard();
			return mp(curCell, 1);
        } else if (cnt == 0) {
			return mp(mp(-1, -1), 0);
		}
    }
	return mp(mp(-1, -1), 1);
}

bool UpdateRows(vector<pair<pii, int> > & updatedCells, int depth) {
    for (int it = 0; it < N; it++) {
        vector<int> nums = getLeftNumbers(rows[it]);
        vector<int> emptyCells = rows[it].getEmptyCells();

        for (int i = 0; i < nums.size(); i++) {
            int amo = 0;
            int curNumber = nums[i];
            vector<pii> Cells;

            for (int j = 0; j < emptyCells.size(); j++) {
                int cell = emptyCells[j];
                int squareId = getSquareId(it, cell);

                if (Can(curNumber, rows[it], cols[cell], squares[squareId])) {
                    amo++;
                    Cells.push_back(make_pair(it, cell));
                }
			}

            pair<pii, bool> chosen = ChooseCell(Cells, curNumber, depth);

            if (!chosen.se || !amo) {
                return 0;
            }

            if (chosen.fi != mp(-1, -1)) {
                updatedCells.push_back(mp(chosen.fi, curNumber));
                
                for (vector<int>::iterator it=emptyCells.begin(); it != emptyCells.end(); it++) {
                    if (*it == chosen.fi.se) {
                        emptyCells.erase(it);
                        break;
                    }
                }
            }
        }
    }

    return 1;
}

bool UpdateCols(vector<pair<pii, int> > & updatedCells, int depth) {
    for (int it = 0; it < N; it++) {
        vector<int> nums = getLeftNumbers(cols[it]);
        vector<int> emptyCells = cols[it].getEmptyCells();

        for (int i = 0; i < nums.size(); i++) {
            int amo = 0;
            int curNumber = nums[i];
            vector<pii> Cells;

            for (int j = 0; j < emptyCells.size(); j++) {
                int cell = emptyCells[j];
                int squareId = getSquareId(cell, it);

                if (Can(curNumber, rows[cell], cols[it], squares[squareId])) {
                    amo++;
                    Cells.push_back(make_pair(cell, it));
                }
            }

            pair<pii, bool> chosen = ChooseCell(Cells, curNumber, depth);

            if (!chosen.se || !amo) {
                return 0;
            }

            if (chosen.fi != mp(-1, -1)) {
                updatedCells.push_back(mp(chosen.fi, curNumber));
                for (vector<int>::iterator it=emptyCells.begin(); it != emptyCells.end(); it++) {
                    if (*it == chosen.fi.fi) {
                        emptyCells.erase(it);
                        break;
                    }
                }
            }

        }
    }

    return 1;
}

bool UpdateSquares(vector<pair<pii, int> > & updatedCells, int depth) {
    for (int it = 0; it < N; it++) {
        vector<int> nums = getLeftNumbers(squares[it]);
        vector<pii> emptyCells = squares[it].getEmptyCells();

        for (int i = 0; i < nums.size(); i++) {
            int amo = 0;
            int curNumber = nums[i];
            vector<pii> Cells;

            for (int j = 0; j < emptyCells.size(); j++) {
                pii cell = getCell(it, emptyCells[j]);

                if (Can(curNumber, rows[cell.first], cols[cell.second], squares[it])) {
                    amo++;
                    Cells.push_back(cell);
                }
            }

            pair<pii, bool> chosen = ChooseCell(Cells, curNumber, depth);

            if (!chosen.se || !amo) {
                return 0;
            }

            if (chosen.fi != mp(-1, -1)) {
                updatedCells.push_back(mp(chosen.fi, curNumber));
                
                for (vector<pii>::iterator iter=emptyCells.begin(); iter != emptyCells.end(); iter++) {
                    if (getCell(it, *iter) == chosen.fi) {
                        emptyCells.erase(iter);
                        break;
                    }
                }
            }
        }
    }

    return 1;
}

bool UpdateAll(int depth, int entryPoint) {
	vector<pair<pii, int> > updatedCells;
	int uCols = 0, uRows = 0, uSquares = 0;
	uCols = UpdateCols(updatedCells, depth);
	if (uCols) {
		uRows = UpdateRows(updatedCells, depth);
		if (uRows)
			uSquares = UpdateSquares(updatedCells, depth);
	}
	if (entryPoint == 1) {
		for (int i = 0; i < updatedCells.size(); i++) {
			resetCell(updatedCells[i].first, updatedCells[i].second);
		}
	}

	return uCols && uRows && uSquares;
}

void putRandomly() {
	for (int it = 0; it < N; it++) {
        vector<int> nums = getLeftNumbers(cols[it]);
        vector<int> emptyCells = cols[it].getEmptyCells();

		for (int i = 0; i < nums.size(); i++) {
            int curNumber = nums[i];

            for (int j = 0; j < emptyCells.size(); j++) {
                int cell = emptyCells[j];
                int squareId = getSquareId(cell, it);

				if (Can(curNumber, rows[cell], cols[it], squares[squareId])) {
					setCell(mp(cell, it), curNumber);
					if (UpdateAll(1, 1)) {
						return;
					} else {
						resetCell(mp(cell, it), curNumber);
					}
                }
            }
		}
	}
}

int main() {
    emptyCellsNumber = ReadInput(rows, cols, squares);

    while (emptyCellsNumber > 0) {
        int tmp = emptyCellsNumber;

        if (!UpdateAll(0, 0)) {
			cout << "Something went wrong.. :(\n";
			break;
		}
        if (tmp == emptyCellsNumber) {
			UpdateAll(MAX_DEPTH, 0);
            if (tmp == emptyCellsNumber) {
                putRandomly();
                if (tmp == emptyCellsNumber) {
                    cout << "This puzzle seems to be impossible\n";
                    break;
                }
            }
        }
    }
	cout << "Result:\n";
	printBoard();
    return 0;
}
