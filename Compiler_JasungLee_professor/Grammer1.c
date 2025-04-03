#include <stdio.h>
#include <stdlib.h>

#define END '$'

char lookahead;

// forward declarations
void nexttoken();
void match(char token);
void list();
void list_prime();
void digit();

void list() {
    digit();
    list_prime();
}

void list_prime() {
    if (lookahead >= '0' && lookahead <= '9') {
        printf("-");
        digit();
        list_prime();
    }
}

void digit() {
    if (lookahead >= '0' && lookahead <= '9') {
        printf("%c", lookahead);
        match(lookahead);
    } else {
        printf(" error");
        exit(1);
    }
}

void match(char token) {
    if (lookahead == token)
        nexttoken();
    else {
        printf(" error");
        exit(1);
    }
}

void nexttoken() {
    int c;
    while (1) {
        c = getchar();
        if (c == ' ' || c == '\t' || c == '\n')
            continue;
        if (c == EOF)
            lookahead = END;
        else
            lookahead = c;
        return;
    }
}

int main() {
    nexttoken();
    if (lookahead == END) {
        printf(" error");
        exit(1);
    }

    list();

    if (lookahead == END)
        printf("\n");
    else
        printf(" error\n");

    return 0;
}
