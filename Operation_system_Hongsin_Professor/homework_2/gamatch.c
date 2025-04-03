#include <stdio.h>
#include <stdlib.h>

int check_direction(int board[6][7], int row, int col, int drow, int dcol) {
    int player = board[row][col];
    int count = 1;
    int r = row + drow, c = col + dcol;
    while (r >= 0 && r < 6 && c >= 0 && c < 7 && board[r][c] == player) {
        count++;
        r += drow;
        c += dcol;
    }
    r = row - drow;
    c = col - dcol;
    while (r >= 0 && r < 6 && c >= 0 && c < 7 && board[r][c] == player) {
        count++;
        r -= drow;
        c -= dcol;
    }
    return count;
}

int check_winner(int board[6][7]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            if (board[i][j] != 0) {
                if (check_direction(board, i, j, 0, 1) >= 4) return board[i][j];
                if (check_direction(board, i, j, 1, 0) >= 4) return board[i][j];
                if (check_direction(board, i, j, 1, 1) >= 4) return board[i][j];
                if (check_direction(board, i, j, 1, -1) >= 4) return board[i][j];
            }
        }
    }
    return 0;
}

int main() {
    int board[6][7];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            scanf("%d", &board[i][j]);
        }
    }
    int winner = check_winner(board);
    if (winner != 0) {
        printf("%d\n", winner);
    } else {
        printf("Not check mate...\n");
    }
    return 0;
}
