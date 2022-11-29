%{

/* Declarations section */
#include <stdio.h>
#include "parser.tab.hpp"
#include "hw3_output.hpp"

%}

%option yylineno
%option noyywrap
digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n \r])
relop           ([<>!]=?|==)
%%

0|[1-9]{digit}*      			return NUM;
void                        return VOID;
int                         return INT;
byte                        return BYTE;
b                           return B;
bool                        return BOOL;
and                         return AND;
or                          return OR;
not                         return NOT;
true                        return TRUE;
false                       return FALSE;
return                      return RETURN;
if                          return IF;
else                        return ELSE;
while                       return WHILE;
break                      return BREAK;
continue                   return CONTINUE;
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN;
\{                          return LBRACE;
\}                          return RBRACE;
{relop}                      return RELOP;
=                           return ASSIGN;
[+-]                          return ADD_MIN;
[*/]                          return MUL_DIV;

{letter}({letter}|{digit})* return ID;
\"([^\n\r\"\\]|\\[rnt"\\])+\" return STRING;
{whitespace}                ;
\/\/[^\r\n]*[ \r|\n|\r\n]? 	;
.                           {output::errorLex(yylineno); exit(0);};

%%
