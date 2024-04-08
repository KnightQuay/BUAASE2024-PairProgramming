// #include <stdio.h>
// #define error 30000
// #define finished 10000
// #define unfinished 20000

int mancalaResult(int flag, int seq[], int size) {
    int board[2][7] = {{4, 4, 4, 4, 4, 4, 0}, {4, 4, 4, 4, 4, 4, 0}};

    int curPlayer = flag - 1;
    for (int i = 0; i < size; i++) {
        int player = seq[i] / 10 - 1;
        int curhole = seq[i] % 10 - 1;

        // illegal instr: wrong player or wrong hole
        if ((player != curPlayer) 
        || (curhole < 0)
        || (curhole >= 6)) {
            return 30000 + i; 
        }

        if(board[curPlayer][curhole] == 0) {
            return 30000 + i;
        }

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
                    curPlayer = curPlayer;
                } else {
                    curPlayer = 1 - curPlayer;
                }
            }
        }

        // check if i + 1 < size but the game is already end
        if (i + 1 < size) {
            int isEndTmp = 1;
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 6; k++) {
                    if (board[j][k] != 0) {
                        isEndTmp = 0;
                        break;
                    }
                }
                if (isEndTmp == 1) {
                    return (30000 + i + 1);
                }
                isEndTmp = 1;
            }   
        }
    }

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
        int fisrtDelta = (flag == 1) ? deltaScore : -deltaScore;
        return 15000 + fisrtDelta;
    } else {
        return 20000 + board[flag - 1][6];
    }
}

// int main() {
//     int seq[100];
//     int size, flag;
//     scanf("%d", &flag);
//     scanf("%d", &size);
//     for (int i = 0; i < size; i++) {
//         scanf("%d", &seq[i]);
//     }
//     printf("%d", mancalaResult(flag, seq, size));
//     // testcase:flag = 1, size = 40, 
//     // seq = 13 16 26 12 16 11 22 25 13 16 15 21 16 14 25 22 
//     // 16 15 24 16 14 23 15 21 16 14 24 12 26 13 16 15 16 14 16 15 25 16 11 26

// }