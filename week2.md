lexel.l
```
%{
#include <stdio.h>
%}

%%

[ \t\r\n]+                                      {}
"//".*                                          {}
"/*"([^*]|\*+[^*/])*\*+"/"                      {}

"int"|"float"|"char"|"if"|"else"|"while"|"return" { printf("KEYWORD: %s\n", yytext); }
[0-9]+"."[0-9]+                                 { printf("FLOAT: %s\n", yytext); }
[0-9]+                                          { printf("INT: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*                          { printf("ID: %s\n", yytext); }
"=="|"!="|"<="|">="|"<"|">"                     { printf("REL_OP: %s\n", yytext); }
"+"|"-"|"*"|"/"|"="                             { printf("OP: %s\n", yytext); }
"("|")"|"{"|"}"|";"|","                         { printf("DELIM: %s\n", yytext); }
.                                               { printf("ERROR: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}
```

Compile and run
```
flex lexer.l
gcc lex.yy.c -o lexer
```

Run:

```
./lexer
```

Example input

```
int main() {
    // calculate sum
    int a = 10;
    int b = 20;

    int sum = a + b;

    return sum;
}
```

Output

```
KEYWORD: int
ID: main
DELIM: (
DELIM: )
DELIM: {
KEYWORD: int
ID: a
OP: =
INT: 10
DELIM: ;
KEYWORD: int
ID: b
OP: =
INT: 20
DELIM: ;
KEYWORD: int
ID: sum
OP: =
ID: a
OP: +
ID: b
DELIM: ;
KEYWORD: return
ID: sum
DELIM: ;
DELIM: }
```