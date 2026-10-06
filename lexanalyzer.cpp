#include <bits/stdc++.h>
using namespace std;

const int MAX_ID_LENGTH = 32;

unordered_set<string> keywords = {
    "if", "else", "while", "for", "return",
    "int", "float", "char", "void",
    "break", "continue"
};

unordered_set<string> operators = {
    "+", "-", "*", "/", "%", "=",
    "==", "!=", "<", ">", "<=", ">=",
    "&&", "||", "!", "++", "--"
};

unordered_set<char> delimiters = {
    '(', ')', '{', '}', '[', ']',
    ';', ',', ':'
};

bool isIdentifierStart(char c) {
    return isalpha(c) || c == '_';
}

bool isIdentifierPart(char c) {
    return isalnum(c) || c == '_';
}

bool isOperatorStart(char c) {
    return string("+-*/%=!<>&|").find(c) != string::npos;
}

int main() {
    string source, line;

    while (getline(cin, line)) {
        source += line + '\n';
    }

    int i = 0;
    int n = source.length();

    while (i < n) {

        if (isspace(source[i])) {
            i++;
            continue;
        }

        if (i + 1 < n && source[i] == '/' && source[i + 1] == '/') {
            i += 2;
            while (i < n && source[i] != '\n')
                i++;
            continue;
        }

        if (i + 1 < n && source[i] == '/' && source[i + 1] == '*') {
            i += 2;

            while (i + 1 < n &&
                   !(source[i] == '*' && source[i + 1] == '/')) {
                i++;
            }

            if (i + 1 < n)
                i += 2;

            continue;
        }

        if (isIdentifierStart(source[i])) {
            string word;

            while (i < n && isIdentifierPart(source[i])) {
                word += source[i];
                i++;
            }

            if (word.length() > MAX_ID_LENGTH) {
                cerr << "Error: Identifier too long\n";
                continue;
            }

            if (keywords.count(word))
                cout << "<KEYWORD, " << word << ">\n";
            else
                cout << "<IDENTIFIER, " << word << ">\n";

            continue;
        }

        if (isdigit(source[i])) {
            string number;
            bool isFloat = false;

            while (i < n && isdigit(source[i])) {
                number += source[i];
                i++;
            }

            if (i < n && source[i] == '.' &&
                i + 1 < n && isdigit(source[i + 1])) {

                isFloat = true;
                number += source[i++];

                while (i < n && isdigit(source[i])) {
                    number += source[i++];
                }
            }

            if (isFloat)
                cout << "<FLOAT, " << number << ">\n";
            else
                cout << "<INTEGER, " << number << ">\n";

            continue;
        }

        if (source[i] == '"') {
            string str;
            str += source[i++];

            bool terminated = false;

            while (i < n) {
                if (source[i] == '\\' && i + 1 < n) {
                    str += source[i++];
                    str += source[i++];
                }
                else if (source[i] == '"') {
                    str += source[i++];
                    terminated = true;
                    break;
                }
                else {
                    str += source[i++];
                }
            }

            if (terminated)
                cout << "<STRING, " << str << ">\n";
            else
                cerr << "Error: Unterminated string\n";

            continue;
        }

        if (source[i] == '\'') {
            string ch;
            ch += source[i++];

            if (i < n && source[i] == '\\') {
                ch += source[i++];

                if (i < n)
                    ch += source[i++];
            }
            else if (i < n) {
                ch += source[i++];
            }

            if (i < n && source[i] == '\'') {
                ch += source[i++];
                cout << "<CHARACTER, " << ch << ">\n";
            }
            else {
                cerr << "Error: Invalid character literal\n";
            }

            continue;
        }

        if (isOperatorStart(source[i])) {
            string op;
            op += source[i];

            if (i + 1 < n) {
                string twoChar;
                twoChar += source[i];
                twoChar += source[i + 1];

                if (operators.count(twoChar)) {
                    op = twoChar;
                    i += 2;
                    cout << "<OPERATOR, " << op << ">\n";
                    continue;
                }
            }

            if (operators.count(op)) {
                i++;
                cout << "<OPERATOR, " << op << ">\n";
                continue;
            }

            cerr << "Error: Invalid operator\n";
            i++;
            continue;
        }

        if (delimiters.count(source[i])) {
            cout << "<DELIMITER, " << source[i] << ">\n";
            i++;
            continue;
        }

        cerr << "Error: Invalid character: " << source[i] << '\n';
        i++;
    }

    return 0;
}