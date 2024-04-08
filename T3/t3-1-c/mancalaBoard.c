// int *mancalaBoard(int flag, int seq[], int size) {
//     static int ans[5];
//     for (int i =0; i < 5; ++i) {
//         ans[i] = 30 - i;
//     }
//     return ans;
// }
int* mancalaBoard(int flag, int seq[], int size) {
    int board[2][7] = {{4, 4, 4, 4, 4, 4, 0}, {4, 4, 4, 4, 4, 4, 0}};
    static int ans[15];
    
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
