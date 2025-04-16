#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

volatile pid_t currentChild = -1;
volatile int currentPlayer = -1;
char game_board[6][7] = {0};

/*void print_board(char arr[6][7]) { //기존코드
    printf("\n@ | A B C D E F G\n");
    printf("-------------------\n");
    for (int i = 0; i < 6; i++) {
        printf("%d | ", 6 - i);
        for (int j = 0; j < 7; j++) {
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }
}*/
void print_board(char arr[6][7]) {
    printf("\n@ | A B C D E F G\n");
    printf("-------------------\n");
    for (int i = 0; i < 6; i++) {
        printf("%d | ", 6 - i);
        for (int j = 0; j < 7; j++) {
            char display = '.';
            if (arr[i][j] == '1') display = 'X';
            else if (arr[i][j] == '2') display = 'Y';
            printf("%c ", display);
        }
        printf("\n");
    }
}


int move(char arr[6][7], char move_do, int player) {
    int col = move_do - 'A';
    if (move_do < 'A' || move_do > 'G') {
        return -1;
    }
    for (int i = 5; i >= 0; i--) {
        if (arr[i][col] == '0') {
            arr[i][col] = player + '0';
            return 1;
        }
    }
    return 0;
}

int check_mate(char arr[6][7]) {
    int x_aixs[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    int y_aixs[8] = {1, 0, -1, -1, -1, 0, 1, 1};

    for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 7; y++) {
            if (arr[x][y] != '0') {
                int check_bit[8] = {0};
                for (int i = 1; i < 4; i++) {
                    for (int j = 0; j < 8; j++) {
                        int rx = x + x_aixs[j] * i;
                        int ry = y + y_aixs[j] * i;
                        if (rx >= 0 && rx < 6 && ry >= 0 && ry < 7 && arr[x][y] == arr[rx][ry]) {
                            check_bit[j]++;
                        }
                    }
                }
                for (int i = 0; i < 8; i++) {
                    if (check_bit[i] >= 3) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void sigint_handler(int signum) {
    printf("\n Ctrl+C 를 눌러 취소되었습니다.\n");
    if (currentChild != -1) {
        kill(currentChild, SIGKILL);
    }
    exit(1);
}

void sigalrm_handler(int signum) {
    if (currentChild != -1) {
        kill(currentChild, SIGKILL);
    }

    if (currentPlayer == 1 || currentPlayer == 2) {
        printf("player %d (%c) timed out! No move within 3 seconds.\n", currentPlayer, currentPlayer == 1 ? 'X' : 'Y');
        printf("player %d (%c) wins by timeout!\n", currentPlayer == 1 ? 2 : 1, currentPlayer == 1 ? 'Y' : 'X');
    } else {
        printf("Unknown player timed out.\n");
    }

    exit(1);
}

int main(int argc, char *argv[]) {
    // 시그널 핸들러 등록
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalrm_handler);

    if (argc != 5 || strcmp(argv[1], "-X") != 0 || strcmp(argv[3], "-Y") != 0) {
        fprintf(stderr, "Usage: ./gamatch -X <agent1> -Y <agent2>\n");
        exit(1);
    }

    memset(game_board, '0', sizeof(game_board));
    int play_turn = 0;

    while (1) {
        int fd1[2], fd2[2];
        pipe(fd1);
        pipe(fd2);

        pid_t pid = fork();

        if (pid == 0) {
            // 자식 프로세스 (에이전트 실행)
            close(fd1[1]);
            close(fd2[0]);

            int agent_num;
            read(fd1[0], &agent_num, sizeof(agent_num));

            dup2(fd1[0], STDIN_FILENO);
            dup2(fd2[1], STDOUT_FILENO);
            close(fd1[0]);
            close(fd2[1]);

            execl(argv[agent_num * 2], argv[agent_num * 2], NULL);
            perror("execl failed");
            exit(1);
        } else {
            // 부모 프로세스
            close(fd1[0]);
            close(fd2[1]);
            currentChild = pid;
            

            int player = play_turn + 1;
            currentPlayer = player;
            char player_char = (player == 1 ? '1' : '2');

            write(fd1[1], &player, sizeof(player));
            char char_buf[2] = { player_char, '\n' };
            write(fd1[1], char_buf, sizeof(char_buf));
            write(fd1[1], game_board, sizeof(game_board));
            close(fd1[1]);

            
            alarm(3);   // 3초 타임아웃 설정 후 에이전트 응답 대기
            char move_char;
            ssize_t n = read(fd2[0], &move_char, 1);
            alarm(0);  // 응답 받으면 타이머 취소
            close(fd2[0]);
            currentChild = -1;

            if (n <= 0) {
                printf("Failed to read move. Agent did not respond in time.\n");
                exit(1);
            }

            int move_result = move(game_board, move_char, player);
            if (move_result == 1) {
                printf("Player %d played move: %c\n", player, move_char);
            } else if (move_result == 0) {
                printf("Column %c is full\n", move_char);
                exit(0);
            } else {
                printf("Player %d made invalid move => '%c'\n", player, move_char);
                exit(0);
            }

            if (check_mate(game_board)) {
                printf("Player %d (%s) wins!\n", player, argv[play_turn == 0 ? 2 : 4]);
                print_board(game_board);
                break;
            }

            print_board(game_board);
            play_turn = (play_turn + 1) % 2;
            sleep(1);
        }
    }

    return 0;
}
