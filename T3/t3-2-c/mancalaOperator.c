#define INFINITY 10000
#define DEPTH 4

void mymemcpy(int *p1, int *p2, int s) {
    for (int i = 0; i < 14; ++i) {
        p1[i] = p2[i];
    }
}

int isGameOver(int seq[]) {
    int isEnd = 1;
    for (int i = 0; i < 6; i++) {
        if (seq[i] != 0) {
            isEnd = 0;
            break;
        }
    }
    if (isEnd == 0) {
        isEnd = 1;
        for (int i = 7; i < 13; i++) {
            if (seq[i] != 0) {
                isEnd = 0;
                break;
            }
        }
    }
    return isEnd;
}

double Evaluate(int seq[], int player) {
    int H1 = seq[player * 7];
    int H2 = player == 0 ?
        seq[0] + seq[1] + seq[2] + seq[3] + seq[4] + seq[5] :
        seq[7] + seq[8] + seq[9] + seq[10] + seq[11] + seq[12];
    int H3 = 0;
    for (int i = player * 7; i < player * 7 + 6; i++) {
        if (seq[i] == (player * 7 + 6 - i))
            H3++;
    }
    int H4 = seq[player == 0 ? 6 : 13]; 
    int H5 = 0;
    for (int i = player * 7 + 5; i >= player * 7; i--) {
        if (seq[i] != 0) {
            H5 = seq[i];
            break;
        }
    }
    int H6 = seq[player == 1 ? 6 : 13]; 

    double W1 = 0.198649, W2 = 0.190084, W3 = 0.370793, W4 = 1, W5 = 0.418841, W6 = 0.565937;
    
    double score = H1*W1 + H2*W2 + H3*W3 + H4*W4 + H5*W5 - H6*W6;
    // printf("%lf\n", score);
    return score;
}

int makeMove(int seq[], int move, int curPlayer) {
    int actMove = (curPlayer == 0) ? move : 7 + move;
    int num = seq[actMove];
    seq[actMove] = 0;
    for (int i = 1; i <= num; i++) {
        if (((i + actMove) % 14) == 6) {
            if (curPlayer == 0)
                seq[6] += 1;
            else {
                num += 1;
                continue;
            }
        } else if (((i + actMove) % 14) == 13) {
            if (curPlayer == 1)
                seq[13] += 1;
            else {
                num += 1;
                continue;
            }
        } else {
            int nextHole = (i + actMove) % 14;
            seq[nextHole] += 1;
        }
    }
    int endIndex = (num + actMove) % 14;
    if ((endIndex != 6) && (endIndex != 13)
    && (seq[endIndex] == 1) && (seq[12 - endIndex] > 0) && ((endIndex / 7) == curPlayer)) {
        int storeIndex = curPlayer * 7 + 6;
        seq[storeIndex] += seq[endIndex];
        seq[storeIndex] += seq[12 - endIndex];
        seq[endIndex] = 0;
        seq[12 - endIndex] = 0;
    }
    if (!((endIndex == 6 && curPlayer == 0)
    || (endIndex == 13 && curPlayer == 1))) {
        curPlayer = 1 - curPlayer;
    }
    return curPlayer;
}


double minmax(double alpha, double beta, int depth, 
int callingPlayer, int seq[], int curPlayer) {
    if (depth == 0 || isGameOver(seq)) {
        return Evaluate(seq, callingPlayer);
    }

    if (curPlayer == callingPlayer) {
        double maxEval = -INFINITY;
        for (int move = 0; move < 6; move++) { 
            if (seq[(curPlayer == 0) ? move : 7 + move] == 0) {
                continue;
            }
            int newBoard[14];
            mymemcpy(newBoard, seq, sizeof(newBoard));
            int nextPlayer = makeMove(newBoard, move, curPlayer);
            double eval = minmax(alpha, beta, depth - 1, callingPlayer, newBoard, nextPlayer);
            maxEval = (eval > maxEval) ? eval : maxEval;
            alpha = (eval > alpha) ? eval : alpha;
            if (beta <= alpha) {
                break; 
            }
        }
        return maxEval;
    } else {
        double minEval = INFINITY;
        for (int move = 0; move < 6; move++) { 
            if (seq[(curPlayer == 0) ? move : 7 + move] == 0) {
                continue;
            }
            int newBoard[14];
            mymemcpy(newBoard, seq, sizeof(newBoard));
            int nextPlayer = makeMove(newBoard, move, curPlayer);
            double eval = minmax(alpha, beta, depth - 1, callingPlayer, newBoard, nextPlayer);
            minEval = (eval < minEval) ? eval : minEval;
            beta = (eval < beta) ? eval : beta;
            if (beta <= alpha) {
                break; 
            }
        }
        return minEval;
    }
} 

int mancalaOperator(int flag, int seq[]) {
    int bestMove = -1;
    double alpha = -INFINITY;
    double beta = INFINITY;
    // double bestScore = ((seq[0] / 10) == flag) ? -INFINITY : INFINITY;
    double bestScore = -INFINITY;
    int curPlayer = flag - 1;

    for (int move = 0; move < 6; ++move) { 
        if (seq[(curPlayer == 0) ? move : 7 + move] == 0) {
            continue;
        }
        int newBoard[14];
        mymemcpy(newBoard, seq, sizeof(newBoard));
        int nextPlayer = makeMove(newBoard, move, curPlayer);
        double score = minmax(alpha, beta, DEPTH, curPlayer, newBoard, nextPlayer);
            
        // if ((((seq[0] / 10) == flag) && score > bestScore) || (((seq[0] / 10) != flag) && score < bestScore)) {
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    return flag * 10 + bestMove + 1; 
}

