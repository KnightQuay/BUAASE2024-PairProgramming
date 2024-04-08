#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
// #include <pair>
using namespace std;


int ans[15] = {0};

int* mancalaBoard(int flag, int seq[], int size) {
    int board[2][7] = {{4, 4, 4, 4, 4, 4, 0}, {4, 4, 4, 4, 4, 4, 0}};
    for (int i = 0; i < 15; i++) {
        ans[i] = 0;
    }

    int curPlayer = (seq[0] / 10) - 1;
    int isLegal = 1;
    for (int i = 0; i < size; i++) {
        int player = seq[i] / 10 - 1;
        int curhole = seq[i] % 10 - 1;

        // new block for size = 1
        if (size == 1) {
            if ((player != (flag - 1)) 
            || (curhole < 0)
            || (curhole >= 6)) {
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 7; j++) {
                        ans[i * 7 + j] = board[i][j];
                    }
                    ans[14] = (flag == 1) ? 152 : 248;
                }
                return ans;
            }
        }
        // new block for size = 1

        // illegal instr: wrong player or wrong hole
        // if ((player != curPlayer) 
        // || (curhole < 0)
        // || (curhole >= 6)) {
        //     return 30000 + i; 
        // }

        // if(board[curPlayer][curhole] == 0) {
        //     return 30000 + i;
        // }

        // curplayer remains unchanged
        int num = board[curPlayer][curhole];
        board[curPlayer][curhole] = 0;
        while (num > 0) {
            curhole = (curhole + 1) % 7;
            if (curhole == 0) {
                player = 1 - player;
            }
            if (curhole != 6 || player == curPlayer) {
                board[player][curhole] += 1;
                num--;
            }
            
            if (num == 0) {
                if (curPlayer == player && curhole <= 5 
                    && board[player][curhole] == 1 && board[1 - player][5 - curhole] > 0) {
                    board[player][6] += (board[player][curhole] + board[1 - player][5 - curhole]);
                    board[player][curhole] = 0;
                    board[1 - player][5 - curhole] = 0;
                } 

                if (curhole == 6) {
                    // curPlayer = curPlayer;
                } else {
                    curPlayer = 1 - curPlayer;
                }
            }
        }
        // new block
        if (i == (size - 2)) {
            if (curPlayer != (flag - 1)) {
                isLegal = 0;
                break;
            } else {
                int isEndTmp = 1;
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 6; k++) {
                        if (board[j][k] != 0) {
                            isEndTmp = 0;
                            break;
                        }
                    }   
                    if (isEndTmp == 1) {
                        isLegal = 0;
                        break;
                    }
                    isEndTmp = 1;
                }
            }
        }
        // new block
    }

    if (isLegal == 0) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 7; j++) {
                ans[i * 7 + j] = board[i][j];
            }
        }
        if (flag == 1) {
            ans[14] = 200 - 48 + 2 * board[0][6];
        } else {
            ans[14] = 200 + 48 - 2 * board[1][6];
        }
        return ans;
    }

    // legal case
    int isEnd = 1;
    for (int i = 0; i < 6; i++) {
        if (board[0][i] != 0) {
            isEnd = 0;
            break;
        }
    }
    if (isEnd == 1) {
        for (int i = 0; i < 6; i++) {
            board[1][6] += board[1][i];
        }
    } else {
        isEnd = 1;
        for (int i = 0; i < 6; i++) {
            if (board[1][i] != 0) {
                isEnd = 0;
                break;
            }
        }
        if (isEnd == 1) {
            for (int i = 0; i < 6; i++) {
                board[0][6] += board[0][i];
            }
        }
    }

    if (isEnd == 1) {
        int deltaScore = board[0][6] - board[1][6];
        ans[6] = board[0][6];
        ans[13] = board[1][6];
        if (deltaScore == 0) {
            ans[14] = 200;
        } else {
            ans[14] = 200 + deltaScore;
        }
    } else {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 7; j++) {
                ans[i * 7 + j] = board[i][j];
            }
        }
        ans[14] = curPlayer + 1;
    }
    return ans;
}


class Hole {
public:
    bool isSpecial;
    int cnt;
    int flag;
    Hole(bool isSpecial, int cnt, int flag) : isSpecial(isSpecial), cnt(cnt), flag(flag) {}
};

class Board { // 棋盘类
public:
    vector<Hole> holes;
    int next; // 表示下一个该谁了
    int bound[3][2]; // 普通洞的上下界
    vector<vector<int> > recs; // 记录双方每次操作后的得分
    Board(int next) : next(next) {
        for (int i = 0; i < 6; ++i) holes.push_back(Hole(false, 4, 1));
        holes.push_back(Hole(true, 0, 1));
        for (int i = 0; i < 6; ++i) holes.push_back(Hole(false, 4, 2));
        holes.push_back(Hole(true, 0, 2));
        bound[1][0] = 0;
        bound[1][1] = 5;
        bound[2][0] = 7;
        bound[2][1] = 12;
        vector<int> t, p;
        recs.push_back(t);
        recs.push_back(p);
    }
    // 13     12 11 10 9 8 7
    // 0 1 2 3 4 5         6
    int getOppoIndex(int i) {
        // 拿到i的对面的下标
        return 12 - i;
    }

    bool isGameOver() {
        int cnt1 = 0, cnt2 = 0;
        for (int i = 0; i < 6; ++i) cnt1 += holes[i].cnt;
        for (int i = 7; i < 13; ++i) cnt2 += holes[i].cnt;
        return (cnt1 == 0 || cnt2 == 0);
    }

    void down(int flag, int i) {
        // flag选手播种了i下标的洞
        // int i = (flag == 1) ? (num - 1) : (6 + num);
        int oppo = (flag == 1) ? 13 : 6; // 对面的计分洞下标
        int self = (flag == 1) ? 6 : 13; // 自己的计分洞的下标
        Hole *h = &holes[i];
        int cur = h->cnt;
        h->cnt = 0;
        i++;
        // 经过对面的计分洞要跳过
        while (cur > 0) {
            if (i == 14) i = 0;
            if (i != oppo) {
                holes[i].cnt++;
                cur--;
            }
            i++;
        }
        i--; // 现在i是最后一个播种下的下标
        

        // 判断是否落入了自己的计分洞
        if (i == self) {} // 不切换播种方直接返回
        // 判断是否触发中门收割: 落在自己的洞 && 落点没棋子 && 对面的有棋子
        else if (i >= bound[flag][0] && i <= bound[flag][1] && holes[i].cnt == 1 && holes[12 - i].cnt > 0) {
            holes[self].cnt += holes[12 - i].cnt;
            holes[self].cnt += holes[i].cnt;
            holes[12 - i].cnt = 0;
            holes[i].cnt = 0;
            next = 3 - flag;
            // next = 3 - flag;
            // return ;
        } else {
            next = 3 - flag;
        }
        // 上面都不满足 或者 满足收割, 都切换
        if (isGameOver()) {
            // cout << "!!!!!!!" << endl;
            // 把双方的棋子都放到自己的棋盘上
            for (int i = 0; i < 6; ++i) { holes[6].cnt += holes[i].cnt; holes[i].cnt = 0; }
            for (int i = 7; i < 13; ++i) { holes[13].cnt += holes[i].cnt; holes[i].cnt = 0; }
        }
    }

    
    
    int generateRightOp() {
        // 根据当前的发球方生成一个正确的操作数, 并执行相应的操作
        int l = bound[next][0], h = bound[next][1];
        int temp = next;
        int ret = next * 10;
        vector<int> indexes;
        for (int i = l; i <= h; ++i) 
            if (holes[i].cnt > 0)
                indexes.push_back(i);
        int i = rand() % indexes.size();
        // cout << "rand-> " << i << " from " << indexes.size() << "is: " << indexes[i] << endl; 
        down(next, indexes[i]);
        ret += ((indexes[i] < 7) ? (indexes[i] + 1) : (indexes[i] - 6));
        // 记录
        recs[0].push_back(getScoreOf(1));
        recs[1].push_back(getScoreOf(2));
        return ret;
    }

    int getScoreOf(int flag) {
        return (flag == 1) ? holes[6].cnt : holes[13].cnt;
    }

    int getExpectRes(int init, int type) { // init表示谁先手
        int self = (init == 1) ? 6 : 13;
        int oppo = (init == 1) ? 13 : 6;
        if (type == 1) {
            // 返回先手的净胜棋数
            return holes[self].cnt - holes[oppo].cnt + 15000;
        } else if (type == 2) {
            // 返回先手的计分洞的棋数
            return holes[self].cnt + 20000;
        }
    }

    void printAll() {
        
        cout << "2-> ";
        for (int i = 13; i >= 7; --i) cout << holes[i].cnt << " ";
        cout << endl << "1-> ";
        for (int i = 0; i < 7; ++i) cout << holes[i].cnt << " ";
        cout << endl << endl;
    }

    int indexMapNum(int i) {
        return (i < 7) ? i + 1 : i - 6;
    }

    int findEmpty(int flag) {
        if (rand() % 10 < 3) return 7;
        // 返回flag选手的一个空的格子编号, 如果都满就返回-1
        vector<int> emptyNums;
        for (int i = bound[flag][0]; i <= bound[flag][1]; ++i) 
            if (holes[i].cnt == 0) emptyNums.push_back(indexMapNum(i));
        if (emptyNums.size() == 0) return -1;
        else return emptyNums[rand() % emptyNums.size()];
    }
};


// int main() {
//     Board b = Board(1);
//     int op[14] = {26, 12, 13, 22, 26, 21, 15, 22, 16, 22, 14, 22, 16, 15};
//     for (int i = 0; i < 14; ++i) {
//         if (i == 13) {
//             int a = 1;
//         }
//         int f = op[i] / 10;
//         int mod = op[i] % 10;
//         int index = (f == 1) ? mod - 1 : 6 + mod;
//         b.down(f, index);
//         b.printAll();
//     }
// }

bool isEqual(vector<int> *ops, bool err) {
    int end = err ? ops->size() - 1 : ops->size();
    vector<int> cpp;
    // 首先求出要返回的数组
    // int* mancalaBoard(int flag, int seq[], int size) {
    if (!err) {
        int *seq = new int[end];
        for (int i = 0; i < end; ++i) seq[i] = (*ops)[i];
        mancalaBoard((*ops)[end - 1] / 10, seq, end);
        // ans已经得到
        Board b = Board((*ops)[0] / 10);
        for (int i = 0; i < end; ++i) {
            int op = (*ops)[i];
            int mod = op % 10;
            int index = (op / 10 == 1) ? mod - 1 : 6 + mod;
            b.down(op / 10, index);
        }
        if (b.isGameOver()) {
            if (b.holes[6].cnt == b.holes[13].cnt) b.holes.push_back(Hole(false, 200, 0));
            else b.holes.push_back(Hole(false, 200 + b.holes[6].cnt - b.holes[13].cnt, 0));
        } else {
            b.holes.push_back(Hole(false, b.next, 0));
        }
        for (int i = 0; i < 15; ++i) {
            if (b.holes[i].cnt != ans[i]) {
                cout << "ans & b.holes" << endl;
                for (int j = 0; j < 15; ++j) cout << ans[j] << " ";
                cout << endl;
                for (int j = 0; j < 15; ++j) cout << b.holes[j].cnt << " ";
                cout << endl;
                return false;
            }
        }
        cout << "ans & b.holes" << endl;
                for (int j = 0; j < 15; ++j) cout << ans[j] << " ";
                cout << endl;
                for (int j = 0; j < 15; ++j) cout << b.holes[j].cnt << " ";
                cout << endl;
        return true;
    }
    return false;
}

int main() {
    srand(time(0));
    int cnt = 1000; // 生成数据的组数, 每一组包括 flag  + n个操作 (前者表示谁先手)
    for (int i = 0; i < cnt; ++i) {
        int type = rand() % 2; // 0 1 2三种类型的数据
        // type = 2;
        int init = rand() % 2 + 1;
        Board b = Board(init);
        vector<int> ops; // 操作序列
        while (!b.isGameOver()) ops.push_back(b.generateRightOp());
        // if (ops.size() > 15) continue;
        // cout << "assert.strictEqual(mancalaResult(" << init << ", [";
        // 现在 ops 是合法操作列表
        vector<int> finalOps;
        bool err = false;
        switch (type) { 
            case 0: { // 游戏刚好结束
                for (int j = 0; j < ops.size(); ++j) {
                    finalOps.push_back(ops[j]);
                    // cout << ops[j];
                    // if (j != ops.size() - 1) cout << ", ";
                    // else cout << "], " << ops.size() << "), " << b.getExpectRes(init, type + 1) << ");" << endl;
                }
            } break;
            case 1: { // 游戏还没结束
                int trunc = rand() % (ops.size() - 1);
                for (int j = 0; j <= trunc; ++j) {
                    finalOps.push_back(ops[j]);
                    // cout << ops[j];
                    // if (j != trunc) cout << ", ";
                    // else cout << "], " << trunc + 1 << "), " << b.recs[init - 1][trunc] + 20000 << ");" << endl;
                }
            } break;
            case 2: { // 有错误操作
                    err = true;
                    /*
                    操作合法: 1 下完了还下
                    操作非法:
                        2 次序错
                        3 次序对, 但是选错
                    */
                    int r = rand() % 2 + 2; // 保证只可能取到[2, 3]
                    int e; // 末尾下标
                    int ei;
                    // r = 1;
                    switch (r) {
                        case 1: {
                            ei = ops.size();
                            int cn = rand() % 10 + 1;
                            for (int i = 0; i < cn; ++i) ops.push_back(10 * (rand() % 2 + 1) + rand() % 6 + 1);
                        } break;
                        case 2: {
                            // 挑一个下标, 把次序改掉
                            int g = rand() % ops.size();
                            ei = g;
                            int s = 3 - ops[g] / 10;
                            ops[g] = s * 10 + ops[g] % 10;
                        } break;
                        case 3: {
                            Board test = Board(ops[0] / 10);
                            vector<pair<int, int> > changes; // 从这里面挑一个修改
                            for (int j = 0; j < ops.size(); ++j) {
                                int f = ops[j] / 10;
                                int mod = ops[j] % 10;
                                int index = (f == 1) ? mod - 1 : 6 + mod;
                                // 记录空的
                                int ret = test.findEmpty(ops[j] / 10);
                                if (ret != -1) changes.push_back( pair<int, int>(j, ret));
                                test.down(f, index);
                            }
                            pair<int, int> p = changes[rand() % changes.size()];
                            int j = p.first;
                            ops[j] = 10 * (ops[j] / 10) + p.second;
                            ei = j;
                        } break;
                    }
                    for (int j = 0; j <= ei; ++j) {
                        finalOps.push_back(ops[j]);
                        // cout << ops[j];
                        // if (j != ops.size() - 1) cout << ", ";
                        // else cout << "], " << ops.size() << "), " << 30000 + ei << ");" << endl;
                    }
            }
        }
        if (!isEqual(&finalOps, err)) {
            cout << "error! [";
            for (int kk = 0; kk < finalOps.size(); ++kk) {
                cout << finalOps[kk] << ", ";
            }
            cout << endl;
            return 0;
        }
    }
    return 0;
}
