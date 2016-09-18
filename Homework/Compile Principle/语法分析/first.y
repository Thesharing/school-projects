%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YYSTYPE double /*double type for YACC stack*/

int yylex(void);
void yyerror(char * s);
int yywrap(void);

%}

%token NUM
%token ID
%left '+' '-'
%left '*' '/'
%right UMINUS
%%


lines : lines expr '\n' {printf("Result: %f\n",$2);}
	|lines '\n'
	|		/* empty */
	|error '\n' {yyerror("Retry please.");
				 yyerrok;}
	|'-' expr %prec UMINUS {$$ = -$2;printf("E->-E\n");}
	;
expr : expr '+' term {$$ = $1 + $3;printf("E->E+T\n");}
	|expr '-' term {$$ = $1 - $3;printf("E->E-T\n");}
	|term {printf("E->T\n");}
	;
term : term '*' form {$$ = $1 * $3;printf("T->T*F\n");}
	|term '/' form {$$ = $1 / $3;printf("T->T/F\n");}
	|form {printf("T->F\n");}
	;
form : ID {printf("F->id\n");}
	|'(' expr ')' {$$ = $2;printf("F->(E)\n");}
	|NUM {printf("F->num\n");}
	;

%%

#include "lex.yy.c"

void yyerror(char * s){
	printf("%s\n",s);
}

int main (){
	return yyparse();
}

