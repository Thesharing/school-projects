%option noyywrap
%{
#include "first.tab.h"
char* id[100];
double num[100];
int count=0;

double install_id();
int yywarp (void);

%}
space		 [' ']*
wrap		 [\n]
letter    	 [A-Za-z]
digit     	 [0-9]
num       	 {digit}+


%%
{num}          {yylval.value=atof(yytext);length=strlen(yytext);return (NUM);}
{space}		   {printf(" ");}
{wrap}		   {return '\n';}
.			   {return yytext[0];} 
%%

int yywarp (void){
	return 1;
}
