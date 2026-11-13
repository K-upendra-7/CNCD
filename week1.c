#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char *keywords[] = {"int", "float", "char", "if", "else", "while", "for", "return", "void"};

int isKeyword(char word[]) {
    for (int i = 0; i < 9; i++)
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    return 0;
}

int main() {
    FILE *fp;
    char filename[50], ch, next, token[100];
    int i;

    printf("Enter input file name: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("File cannot be opened.\n");
        return 0;
    }

    printf("\nTokens:\n");

    while ((ch = fgetc(fp)) != EOF) {

        if (isspace(ch))
            continue;

        if (ch == '/') {
            next = fgetc(fp);

            if (next == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                continue;
            }

            if (next == '*') {
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '*' && (next = fgetc(fp)) == '/')
                        break;
                    if (ch == '*')
                        ungetc(next, fp);
                }
                continue;
            }

            ungetc(next, fp);
            printf("Operator: /\n");
            continue;
        }

        if (isalpha(ch) || ch == '_') {
            i = 0;

            do {
                if (i < MAX)
                    token[i++] = ch;
                ch = fgetc(fp);
            } while (isalnum(ch) || ch == '_');

            token[i] = '\0';
            ungetc(ch, fp);

            printf("%s: %s\n", isKeyword(token) ? "Keyword" : "Identifier", token);
        }

        else if (isdigit(ch)) {
            i = 0;

            do {
                token[i++] = ch;
                ch = fgetc(fp);
            } while (isdigit(ch) || ch == '.');

            token[i] = '\0';
            ungetc(ch, fp);

            printf("Number: %s\n", token);
        }

        else if (strchr("+-*%=<>", ch)) {
            next = fgetc(fp);

            if ((ch == '+' && next == '+') || (ch == '-' && next == '-') || (strchr("=<>", ch) && next == '=')) {
                printf("Operator: %c%c\n", ch, next);
            } else {
                printf("Operator: %c\n", ch);
                ungetc(next, fp);
            }
        }

        else if (strchr(";,(){}[]", ch))
            printf("Symbol: %c\n", ch);

        else
            printf("Unknown: %c\n", ch);
    }

    fclose(fp);
    return 0;
}
