#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>     // strdup 함수 선언
#include <sys/wait.h>   // wait 함수 선언

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // 자식: 표준 출력 파이프 재지정
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        char *myargs[3];
        myargs[0] = strdup("wc");    // 프로그램: "wc"
        myargs[1] = strdup("p4.c");  // wc 대상 파일
        myargs[2] = NULL;            // 인자 목록 종료
        execvp(myargs[0], myargs);
    } else {
        // 부모
        int rc_wait = wait(NULL);
    }
    return 0;
}
