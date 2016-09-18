%{
#include <stdio.h>

int wordCount=0;
int charCount=0;
int columnCount=0;
int lineCount=1;
int IDCount=0;
int stringCount=0;

%}

delim     	 [' '\t]+
wrap      	 [\n]
notwrap   	 [^\n]
ws        	 {delim}+
letter    	 [A-Za-z]
digit     	 [0-9]
notpunc		 [^\"]
keyword		 char|short|int|unsigned|long|float|double|struct|union|void|enum|signed|const|volatile|typedef|auto|register|static|extern|break|case|continue|default|do|else|for|goto|if|return|switch|while|sizeof
punc		 \+|\-|\*|\\|\&|\(|\)|\{|\}|\[|\]|=|==|\||\^|\?|\.|\,|<|>|;
id        	 {letter}({letter}|{digit})*
num       	 {digit}+(\.{digit}+)?
str     	 \"{notwrap}*\"
linecomment      \/\/{notwrap}*
error1   	 {digit}+{letter}+({letter}|{digit})*
error2           \"{notpunc}*
error3           @|~|`
%x        	 IN_COMMENT

%%
{delim}        {columnCount+=yyleng;charCount+=yyleng;}
{keyword}      {printf("KEYWORD: %s\n",yytext);columnCount+=yyleng;charCount+=yyleng;wordCount++;}
{id}           {IDCount++;printf("ID%d: %s\n",IDCount,yytext);columnCount+=yyleng;charCount+=yyleng;wordCount++;}
{num}          {printf("NUM: %s\n",yytext);columnCount+=yyleng;charCount+=yyleng;}
{linecomment}  {printf("LINENOTE: %s\n",yytext);}
{str}          {stringCount++;printf("STRING%d: %s\n",stringCount,yytext);columnCount+=yyleng;charCount+=yyleng;}
{punc}	       {printf("PUNCTUATION: %s\n",yytext);columnCount++;charCount++;}
{error1}       {printf("ERROR: Line %d, Column %d: Identity cannot start with number.\"%s\"\n",lineCount,columnCount+1,yytext);columnCount+=yyleng;charCount+=yyleng;}
{error3}       {printf("ERROR: Line %d, Column %d: Invalid Symbol.\"%s\"\n",lineCount,columnCount+1,yytext);columnCount+=yyleng;charCount+=yyleng;}

<INITIAL>{
"/*"           BEGIN(IN_COMMENT);yymore();
}
<IN_COMMENT>{
"*/"           printf("ANNODATE: %s\n",yytext);BEGIN(INITIAL);
[^*\n]+	       yymore();
"*"            yymore();
\n             lineCount++;yymore();
}
	   
{error2}       {printf("ERROR:Line %d, Column %d: Cannot Match the Left Puncuation.\n",lineCount,columnCount+1);columnCount+=yyleng;charCount+=yyleng;}
{wrap}         {lineCount++;columnCount=0;}
.              {printf("%s",yytext);charCount++;columnCount++;}

%%



int main(){
	yylex();
	printf("\nFinished.\nResult:\n");
	printf("Chars: %d \n",charCount);
	printf("Lines: %d \n",lineCount);
	printf("Words: %d \n",wordCount);
	return 0;
}

int yywrap(){
	return 1;
}
