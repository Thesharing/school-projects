%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
struct exp{
	double value;
	int type;
};
int length;
#define YYSTYPE struct exp

int yylex(void);
void yyerror(char * s);
int yywrap(void);

%}

%token NUM
%left '+' '-'
%left '*' '/'
%right UMINUS
%%


lines : lines expr '\n' {if($2.type == 0){
							 printf("Type: Int\n");
							 printf("Result: %d\n",(int)$2.value);
						 }
						 else {
						 	 printf("Type: Real\n");
						 	 printf("Result: %f\n",$2.value);
						 }}
	|lines '\n'
	|		/* empty */
	|error '\n' {yyerror("Retry please.");
				 yyerrok;}
	|'-' expr %prec UMINUS {$$.type = $2.type;
							$$.value = -$2.value;
							printf("E->-E\n");}
	;
expr : expr '+' term {if($1.type == 0 && $3.type == 0)
					  	  $$.type = 0;
					  else
					  	  $$.type = 1;
					  $$.value = $1.value + $3.value;
					  printf("E->E+T\n");}
	|expr '-' term {if($1.type == 0 && $3.type == 0)
					  	$$.type = 0;
					else
					  	$$.type = 1;
					$$.value = $1.value - $3.value;
					printf("E->E-T\n");}
	|term {$$.type = $1.type;
		   $$.value = $1.value;
		   printf("E->T\n");}
	;
term : term '*' form {if($1.type == 0 && $3.type == 0)
					  	$$.type = 0;
					  else
					  	$$.type = 1;
					  $$.value = $1.value * $3.value;
					  printf("T->T*F\n");}
	|term '/' form {if($1.type == 0 && $3.type == 0){
					  	$$.type = 0;
					  	$$.value = (int)($1.value / $3.value);
					}
					else{
					  	$$.type = 1;
					  	$$.value = $1.value / $3.value;
					}
					printf("T->T/F\n");}
	|form {$$.type = $1.type;
		   $$.value = $1.value;
		   printf("T->F\n");}
	;
form : NUM '.' NUM {$$.type = 1;
					$$.value = $1.value + $3.value / pow(10,(double)length);printf("F->num.num\n");}
	|'(' expr ')' {$$.type = $2.type;
				   $$.value = $2.value;printf("F->(E)\n");}
	|NUM {$$.type = 0;
		  printf("F->num\n");}
	;

%%

#include "lex.yy.c"

void yyerror(char * s){
	printf("%s\n",s);
}

int main (){
	return yyparse();
}

