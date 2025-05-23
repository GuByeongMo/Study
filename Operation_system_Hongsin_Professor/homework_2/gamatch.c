#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t currentChild = -1;
int currentPlayer_num = -1;
char game_board[6][7] = {0};

void print_board(char arr[6][7])
{
    printf("\n@ | A B C D E F G\n");
    printf("-------------------\n");
    for (int i = 0; i < 6; i++)
    {
        printf("%d | ", 6 - i);
        for (int j = 0; j < 7; j++)
        {
            char display = '.';
            if (arr[i][j] == '1')
                display = 'X';
            else if (arr[i][j] == '2')
                display = 'Y';
            printf("%c ", display);
        }
        printf("\n");
    }
}

int move(char arr[6][7], char move_do, int player_num)
{
    int col = move_do - 'A';
    if (move_do < 'A' || move_do > 'G')
    {
        return -1;
    }
    for (int i = 5; i >= 0; i--)
    {
        if (arr[i][col] == '0')
        {
            arr[i][col] = player_num + '0';
            return 1;
        }
    }
    return 0;
}

int check_mate(char arr[6][7])
{
    int x_aixs[8] = {1, 1, 1, 0, -1, -1, -1, 0};
    int y_aixs[8] = {1, 0, -1, -1, -1, 0, 1, 1};
    int check_sum=0;

    for (int x = 0; x < 6; x++)
    {
        for (int y = 0; y < 7; y++)
        {
            if (arr[x][y] != '0')
            {
                check_sum++;
                int check_bit[8] = {0};
                for (int i = 1; i < 4; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        int rx = x + x_aixs[j] * i;
                        int ry = y + y_aixs[j] * i;
                        if (rx >= 0 && rx < 6 && ry >= 0 && ry < 7 && arr[x][y] == arr[rx][ry])
                        {
                            check_bit[j]++;
                        }
                    }
                }
                for (int i = 0; i < 8; i++)
                {
                    if (check_bit[i] >= 3)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    if(check_sum==42){
        return -1;
    }
    else return 0;
}

void sigint_handler(int signum)
{
    printf("\n Ctrl+C 를 눌러 취소되었습니다.\n");
    if (currentChild != -1)
    {
        kill(currentChild, SIGKILL);
    }
    exit(1);
}

void sigalrm_handler(int signum)
{
    if (currentChild != -1)
    {
        kill(currentChild, SIGKILL);
    }

    if (currentPlayer_num == 1 || currentPlayer_num == 2)
    {
        printf("player_num %d (%c) timed out! No move within 3 seconds.\n", currentPlayer_num, currentPlayer_num == 1 ? 'X' : 'Y');
        printf("player_num %d (%c) wins by timeout!\n", currentPlayer_num == 1 ? 2 : 1, currentPlayer_num == 1 ? 'Y' : 'X');
    }
    else
    {
        printf("Unknown player_num timed out.\n");
    }

    exit(1);
}

int main(int argc, char *argv[])
{
    // 시그널 핸들러 등록
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalrm_handler);

    if (argc != 5 || strcmp(argv[1], "-X") != 0 || strcmp(argv[3], "-Y") != 0)
    {
        fprintf(stderr, "Usage: ./gamatch -X <agent1> -Y <agent2>\n");
        exit(1);
    }

    memset(game_board, '0', sizeof(game_board));
    int play_turn = 0;

    while (1)
    {
        int to_agent[2], from_agent[2];
        pipe(to_agent);
        pipe(from_agent);

        pid_t pid = fork();
        if(pid<0){
            printf("fork fail...\n");
            exit(1);
        }
        else if (pid == 0)
        {
            // 자식 프로세스 (에이전트 실행)
            close(to_agent[1]);
            close(from_agent[0]);

            int agent_num;
            read(to_agent[0], &agent_num, sizeof(agent_num));

            dup2(to_agent[0], STDIN_FILENO);
            dup2(from_agent[1], STDOUT_FILENO);
            close(to_agent[0]);
            close(from_agent[1]);

            execl(argv[agent_num * 2], argv[agent_num * 2], NULL);
            perror("execl failed");
            exit(1);
        }
        else
        {
            // 부모 프로세스
            close(to_agent[0]); close(from_agent[1]);
            currentChild = pid;

            int player_num = (play_turn++)%2 + 1;
            write(to_agent[1], &player_num, sizeof(player_num));
            
            
            currentPlayer_num = player_num;
            char player_char = (player_num == 1 ? '1' : '2');
            char char_buf[2] = {player_char, '\n'};
            write(to_agent[1], char_buf, sizeof(char_buf));
            //write(to_agent[1], game_board, sizeof(game_board));
            // 기존 board 전송 제거하고 아래처럼 변경
            for (int i = 0; i < 6; i++)
            {
                char row[32];
                int offset = 0;
                for (int j = 0; j < 7; j++)
                {
                    row[offset++] = game_board[i][j];
                    if (j < 6)
                        row[offset++] = ' ';
                }
                row[offset++] = '\n';
                row[offset] = '\0';
                write(to_agent[1], row, strlen(row));
            }

            close(to_agent[1]);

            alarm(3); // 3초 타임아웃 설정 후 에이전트 응답 대기
            char move_char;
            waitpid(pid, NULL, 0); // 자식 프로세스 종료 대기
            if (read(from_agent[0], &move_char, 1) <= 0)
            {
                printf("Failed to read move. Agent did not respond in time.\n");
                exit(1);
            }
            alarm(0); // 응답 받으면 타이머 취소
            close(from_agent[0]);
            currentChild = -1;

            int move_result = move(game_board, move_char, player_num);
            if (move_result == 1)
            {
                printf("\nPlayer_num %d played move: %c\n", player_num, move_char);
                printf("TURN : %d", play_turn);
            }
            else if (move_result == 0)
            {
                printf("Column %c is full\n", move_char);
                exit(0);
            }
            else
            {
                printf("Player_num %d made invalid move => '%c'\n", player_num, move_char);
                exit(0);
            }

            int check_mate_result = check_mate(game_board);
            if (check_mate_result == 1)
            {
                printf("\nPlayer_num %d (%s) wins!", player_num, argv[player_num == 0 ? 2 : 4]);
                print_board(game_board);
                exit(0);
            }
            else if(check_mate_result==-1) 
            {
                printf("Draw...\n");
                print_board(game_board);
                exit(0);
            }

            print_board(game_board);
            sleep(1);
        }
    }

    return 0;
}
