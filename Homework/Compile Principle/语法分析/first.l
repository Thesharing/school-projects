%option noyywrap
%{
#include "first.tab.h"
char* id[100];
double num[100];
int count=0;
extern double yylval;

double install_id();
int yywarp (void);

%}
space		 [' ']*
wrap		 [\n]
letter    	 [A-Za-z]
digit     	 [0-9]
id        	 {letter}({letter}|{digit})*
num       	 {digit}+(\.{digit}+)?


%%
{id}           {yylval=install_id();return (ID);}
{num}          {yylval=atof(yytext);return (NUM);}
{space}		   {printf(" ");}
{wrap}		   {return '\n';}
.			   {return yytext[0];} 
%%

double install_id(){
	int i=0;
	for(i=0;i<count;i++){
		if(strcmp(yytext,id[i])==0){
			return num[i];
		}
	}
	printf("Found an unassigned id:\n%s = ",yytext);
	id[count]=(char*)malloc(sizeof(char)*yyleng);
	strcpy(id[count],yytext);
	scanf("%lf", &num[count]);
	count++;
	return num[count-1];
}

int yywarp (void){
	return 1;
}
