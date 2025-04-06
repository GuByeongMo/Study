#include <stdio.h>
#include <stdlib.h>

void nexttoken();
void match(char token);
void list();
void digit();
void list_dash();

char lookahead;

int main() 
{
    nexttoken();
    list();
    if (lookahead == '$')
    {
        printf(" \n");    //그냥 출력
    }
    else
    {
        printf(" error\n");  //에러가 있는걸 확인할 수 있게 출력
    }
    return 0;
}
void nexttoken() 
{
    int c;
    while (1) 
    {
        c = getchar();
        if (c == ' ' || c == '\t' || c == '\n')
        {
            continue;     
        }

        if (c == EOF)
        {
            lookahead = '$';
        }

        else
        {
            lookahead = c;
        }
        return;
    }
}

void match(char token) 
{
    if (lookahead == token)
    {
        nexttoken();
    }
    else 
    {
        printf(" error");
        exit(1);
    }
}

void list() 
{
    digit();
    list_dash();
}



void digit() 
{
    if (lookahead >= '0' && lookahead <= '9') 
    {
        printf("%c", lookahead);
        match(lookahead);
    } 
    else 
    {
        printf(" error");
        exit(1);
    }
}

void list_dash() 
{
    if (lookahead >= '0' && lookahead <= '9') 
    {
        printf("-");
        digit();
        list_dash();
    }
}


