#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char game_board[6][7] = {0};

int main(int argc, char *argv[]) {
    int play_turn = 0;

    while(1){
        int fd1[2], fd2[2];
        pipe(fd1);  pipe(fd2);

        pid_t pid = fork();
        
        if (pid == 0) {
            close(fd1[1]); close(fd2[0]);

            int agent_num;
            read(fd1[0], &agent_num, sizeof(agent_num));
            dup2(fd1[0], 0);
            close(fd1[0]);  
            dup2(fd2[1], 1);
            close(fd2[1]);

            execl(argv[agent_num*2], argv[agent_num*2], NULL);
            perror("execl failed");
            exit(1);
        }
        else {
            int player = play_turn+1;
            char player_num = (player == 1? '1': '2');
            close(fd1[0]);  close(fd2[1]);
            write(fd1[1], &player, sizeof(player));
            write(fd1[1], &player_num, sizeof(player_num));
            write(fd1[1], game_board, sizeof(game_board));
            close(fd1[1]);

            wait(NULL);
            char receive[2];
            read(fd2[0], receive, sizeof(receive));
            //
            printf("receive : %s\n", receive);
            //
            play_turn = (play_turn + 1) % 2;
            sleep(3);
        }
    }
    return 0;
}
