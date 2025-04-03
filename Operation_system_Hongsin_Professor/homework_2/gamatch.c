#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#define STACKS 7
#define LEVELS 6

// 게임판 상태 저장
int board[LEVELS][STACKS];

// 보드 출력 함수
void print_board() {
    printf("\n  A B C D E F G\n");
    for (int i = 0; i < LEVELS; i++) {
        printf("%d ", LEVELS - i);
        for (int j = 0; j < STACKS; j++) {
            char mark = '.';
            if (board[i][j] == 1) mark = 'X';
            if (board[i][j] == 2) mark = 'Y';
            printf("%c ", mark);
        }
        printf("\n");
    }
    printf("\n");
}

// 보드에 돌 추가
int apply_move(char move, int player) {
    int col = move - 'A';
    if (col < 0 || col >= STACKS) return -1;
    for (int i = LEVELS - 1; i >= 0; i--) {
        if (board[i][col] == 0) {
            board[i][col] = player;
            return 0;
        }
    }
    return -1; // 가득 찬 경우
}

// 승리 여부 확인
int check_win(int player) {
    for (int i = 0; i < LEVELS; i++) {
        for (int j = 0; j < STACKS; j++) {
            if (board[i][j] != player) continue;
            // 오른쪽
            if (j <= STACKS - 4 && board[i][j+1] == player && board[i][j+2] == player && board[i][j+3] == player)
                return 1;
            // 아래
            if (i <= LEVELS - 4 && board[i+1][j] == player && board[i+2][j] == player && board[i+3][j] == player)
                return 1;
            // 대각선 ↘
            if (i <= LEVELS - 4 && j <= STACKS - 4 && board[i+1][j+1] == player && board[i+2][j+2] == player && board[i+3][j+3] == player)
                return 1;
            // 대각선 ↙
            if (i <= LEVELS - 4 && j >= 3 && board[i+1][j-1] == player && board[i+2][j-2] == player && board[i+3][j-3] == player)
                return 1;
        }
    }
    return 0;
}

// agent 실행 및 move 읽기
char run_agent(const char *agent_path, int player_num) {
    int to_agent[2], from_agent[2];
    pipe(to_agent);
    pipe(from_agent);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(to_agent[0], STDIN_FILENO);
        dup2(from_agent[1], STDOUT_FILENO);
        close(to_agent[1]); close(from_agent[0]);
        execl(agent_path, agent_path, NULL);
        perror("exec failed");
        exit(1);
    } else {
        close(to_agent[0]); close(from_agent[1]);

        dprintf(to_agent[1], "%d\n", player_num);
        for (int i = 0; i < LEVELS; i++) {
            for (int j = 0; j < STACKS; j++) {
                dprintf(to_agent[1], "%d ", board[i][j]);
            }
            dprintf(to_agent[1], "\n");
        }
        close(to_agent[1]);

        char move;
        read(from_agent[0], &move, 1);
        close(from_agent[0]);

        waitpid(pid, NULL, 0);
        return move;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[1], "-X") != 0 || strcmp(argv[3], "-Y") != 0) {
        fprintf(stderr, "Usage: %s -X <agent1> -Y <agent2>\n", argv[0]);
        exit(1);
    }

    const char *agent1 = argv[2];
    const char *agent2 = argv[4];
    int turn = 0;

    memset(board, 0, sizeof(board));

    while (1) {
        print_board();
        int player = (turn % 2 == 0) ? 1 : 2;
        const char *agent = (player == 1) ? agent1 : agent2;

        char move = run_agent(agent, player);
        printf("Player %c chooses: %c\n", (player == 1) ? 'X' : 'Y', move);

        if (apply_move(move, player) != 0) {
            printf("Invalid move! Player %d loses.\n", player);
            break;
        }

        if (check_win(player)) {
            print_board();
            printf("Player %c wins!\n", (player == 1) ? 'X' : 'Y');
            break;
        }

        if (turn >= STACKS * LEVELS - 1) {
            print_board();
            printf("Draw game.\n");
            break;
        }

        sleep(1);
        turn++;
    }

    return 0;
}
