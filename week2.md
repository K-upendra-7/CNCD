Compile and run

If you are using Flex + GCC:

```
flex lexer.l
gcc lex.yy.c -o lexer
```

Run:

```
./lexer
```

On Windows with Flex/Bison installed:

```
flex lexer.l
gcc lex.yy.c -o lexer.exe
lexer.exe
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
<KEYWORD, int>
<IDENTIFIER, main>
<DELIMITER, (>
<DELIMITER, )>
<DELIMITER, {>
<KEYWORD, int>
<IDENTIFIER, a>
<OPERATOR, =>
<INTEGER, 10>
<DELIMITER, ;>
<KEYWORD, int>
<IDENTIFIER, b>
<OPERATOR, =>
<INTEGER, 20>
<DELIMITER, ;>
<KEYWORD, int>
<IDENTIFIER, sum>
<OPERATOR, =>
<IDENTIFIER, a>
<OPERATOR, +>
<IDENTIFIER, b>
<DELIMITER, ;>
<KEYWORD, return>
<IDENTIFIER, sum>
<DELIMITER, ;>
<DELIMITER, }>
```