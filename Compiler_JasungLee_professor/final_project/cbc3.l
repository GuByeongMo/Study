%{
#include <string.h>
#include "cbc3.h"
#define MAXSYM	100
#define	MAXSYMLEN	20
char symtbl[MAXSYM][MAXSYMLEN];
int lineno=1;
int insertsym();
int maxsym=0;
char s[MAXSYMLEN];
int temp;
int has_honorific = 0;  // 존댓말 사용 여부를 체크하는 변수

%}
sp		[ \t]
ws		{sp}+
nl		\n
eletter		[A-Za-z]
hletter		[\xb0-\xfe][\xa0-\xfe]
letter		({eletter}|{hletter})
digit		[0-9]
id		{letter}({letter}|{digit})*
honorific	("습니다"|"니다"|"세요"|"시"|"입니다")

%%
{ws}		{/* do nothing */}
{nl}		{lineno++; }
\+		{return(ADD); }
\-		{return(SUB); }
\=		{return(ASSGN); }
\(		{return('('); }
\)		{return(')'); }
\{		{return('{'); }
\}		{return('}'); }
;		{return(STMTEND); }
main		{return(MAIN); }
{honorific}	{has_honorific = 1; /* 존댓말 발견 */ }
{id}		{temp=insertsym(yytext); yylval=MakeNode(ID, temp); return(ID);}
-?{digit}+	{sscanf(yytext, "%d", &temp); yylval=MakeNode(NUM, temp); return(NUM);}
.		{printf("잘못된 토큰입니다: %s\n", yytext); }
%%

int insertsym(s)
char *s;
{
int i;

	for(i=0;i<maxsym; i++) {
		if(strcmp(s, symtbl[i])==0) return(i);
		}
	if (i<MAXSYM-1) {
		strcpy(symtbl[maxsym],s);
		maxsym++;
		return(maxsym-1);
		}
	else
		{printf("심볼 테이블이 가득 찼습니다.\n");}
	return(0);
}

yywrap() {
    if (!has_honorific) {
        printf("오류: 존댓말을 사용하지 않았습니다. 컴파일을 중단합니다.\n");
        return -1;  // 컴파일 중단
    }
    return 1;
}