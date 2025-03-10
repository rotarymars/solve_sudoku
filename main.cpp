#include <iostream>
#include <set>
#include <vector>

using namespace std;

void remove_except_n(set<int> &s, int n) {
  for (auto it = s.begin(); it != s.end();) {
    if (*it != n)
      it = s.erase(it);
    else
      ++it;
  }
}

bool propagate(vector<vector<set<int>>> &board) {
  bool flag;
  int dh[] = {0, 3, 6}, dw[] = {0, 3, 6};
  do {
    flag = false;
    for (int i = 0; i < 9; ++i) {
      set<int> already;
      vector<int> numbercnt(9, 0);
      for (int j = 0; j < 9; ++j) {
        if (board[i][j].size() == 1)
          already.insert(*board[i][j].begin());
        for (auto k : board[i][j])
          ++numbercnt[k - 1];
      }
      for (int j = 0; j < 9; ++j) {
        if (board[i][j].size() != 1) {
          for (auto k : already) {
            if (board[i][j].count(k)) {
              board[i][j].erase(k);
              flag = true;
            }
          }
        }
      }
      for (int j = 0; j < 9; ++j) {
        if (numbercnt[j] == 1) {
          for (int k = 0; k < 9; ++k) {
            if (board[i][k].count(1 + j))
              remove_except_n(board[i][k], 1 + j);
          }
        }
      }
    }
    for (int i = 0; i < 9; ++i) {
      set<int> already;
      vector<int> numbercnt(9, 0);
      for (int j = 0; j < 9; ++j) {
        if (board[j][i].size() == 1)
          already.insert(*board[j][i].begin());
        for (auto k : board[j][i])
          ++numbercnt[k - 1];
      }
      for (int j = 0; j < 9; ++j) {
        if (board[j][i].size() != 1) {
          for (auto k : already) {
            if (board[j][i].count(k)) {
              board[j][i].erase(k);
              flag = true;
            }
          }
        }
      }
      for (int j = 0; j < 9; ++j) {
        if (numbercnt[j] == 1) {
          for (int k = 0; k < 9; ++k) {
            if (board[k][i].count(1 + j))
              remove_except_n(board[k][i], 1 + j);
          }
        }
      }
    }
    for (int bi = 0; bi < 3; ++bi) {
      for (int bj = 0; bj < 3; ++bj) {
        set<int> already;
        vector<int> numbercnt(9, 0);
        for (int i = dh[bi]; i < dh[bi] + 3; ++i) {
          for (int j = dw[bj]; j < dw[bj] + 3; ++j) {
            if (board[i][j].size() == 1)
              already.insert(*board[i][j].begin());
            for (auto m : board[i][j])
              ++numbercnt[m - 1];
          }
        }
        for (int i = dh[bi]; i < dh[bi] + 3; ++i) {
          for (int j = dw[bj]; j < dw[bj] + 3; ++j) {
            if (board[i][j].size() != 1) {
              for (auto m : already) {
                if (board[i][j].count(m)) {
                  board[i][j].erase(m);
                  flag = true;
                }
              }
            }
          }
        }
        for (int j = 0; j < 9; ++j) {
          if (numbercnt[j] == 1) {
            for (int i = dh[bi]; i < dh[bi] + 3; ++i) {
              for (int k = dw[bj]; k < dw[bj] + 3; ++k) {
                if (board[i][k].count(1 + j))
                  remove_except_n(board[i][k], 1 + j);
              }
            }
          }
        }
      }
    }
    for (int i = 0; i < 9; ++i)
      for (int j = 0; j < 9; ++j)
        if (board[i][j].empty())
          return false;
  } while (flag);
  return true;
}

bool isSolved(const vector<vector<set<int>>> &board) {
  for (int i = 0; i < 9; ++i)
    for (int j = 0; j < 9; ++j)
      if (board[i][j].size() != 1)
        return false;
  return true;
}

bool solveSudoku(vector<vector<set<int>>> &board) {
  if (!propagate(board))
    return false;

  if (isSolved(board))
    return true;

  int row = -1, col = -1, minOptions = 10;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int options = board[i][j].size();
      if (options > 1 && options < minOptions) {
        minOptions = options;
        row = i;
        col = j;
      }
    }
  }
  set<int> possibilities = board[row][col];
  for (int val : possibilities) {
    auto boardCopy = board;
    remove_except_n(boardCopy[row][col], val);
    if (solveSudoku(boardCopy)) {
      board = boardCopy;
      return true;
    }
  }
  return false;
}

int main() {
  vector<vector<set<int>>> board(
      9, vector<set<int>>(9, {1, 2, 3, 4, 5, 6, 7, 8, 9}));
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      char c;
      cin >> c;
      if (c == '?')
        continue;
      for (int k = 1; k <= 9; ++k) {
        if (c != '0' + k)
          board[i][j].erase(k);
      }
    }
  }

  if (solveSudoku(board)) {
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        char c = (board[i][j].size() == 1 ? '0' + *board[i][j].begin() : '?');
        if (j > 0)
          cout << ' ';
        cout << c;
      }
      cout << '\n';
    }
  } else {
    cout << "No solution found.\n";
  }
  return 0;
}
