%{
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap

KEYWORD     (if|else|while|for|return|int|float|char|void|break|continue)
ID          [a-zA-Z_][a-zA-Z0-9_]*
INTEGER     [0-9]+
FLOAT       [0-9]+\.[0-9]+
STRING      \"([^\"\\]|\\.)*\"
CHAR        \'([^\'\\]|\\.)\'

%%

[ \t\n]+                    ;

"//".*                      ;

"/*"([^*]|\*+[^*/])*\*+"/"  ;

{KEYWORD}                   { printf("<KEYWORD, %s>\n", yytext); }

{FLOAT}                     { printf("<FLOAT, %s>\n", yytext); }

{INTEGER}                   { printf("<INTEGER, %s>\n", yytext); }

{STRING}                    { printf("<STRING, %s>\n", yytext); }

{CHAR}                      { printf("<CHARACTER, %s>\n", yytext); }

{ID}                        {
                                if (yyleng > 32)
                                    printf("Error: Identifier too long\n");
                                else
                                    printf("<IDENTIFIER, %s>\n", yytext);
                            }

"=="|"!="|"<="|">="|"&&"|"||"|"++"|"--" {
                                printf("<OPERATOR, %s>\n", yytext);
                            }

"+"|"-"|"*"|"/"|"%"|"="|"!"|"<"|">" {
                                printf("<OPERATOR, %s>\n", yytext);
                            }

"("|")"|"{"|"}"|"["|"]"|";"|","|":" {
                                printf("<DELIMITER, %s>\n", yytext);
                            }

.                           {
                                printf("Error: Invalid character %s\n", yytext);
                            }

%%