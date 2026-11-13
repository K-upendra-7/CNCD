#include <bits/stdc++.h>
using namespace std;

const int MAX_LEN = 20;

const vector<string> keywords = {
    "int", "float", "char", "if", "else", "while", "for", "return", "void"
};

bool isKeyword(const string& word) {
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

int main() {
    string filename;
    cout << "Enter input file name: ";
    cin >> filename;

    ifstream fp(filename);

    if (!fp.is_open()) {
        cout << "File cannot be opened.\n";
        return 0;
    }

    cout << "\nTokens:\n";

    char ch, next;
    string operators = "+-*%=<>";
    string symbols = ";,(){}[]";

    while (fp.get(ch)) {
        if (isspace(static_cast<unsigned char>(ch)))
            continue;

        if (ch == '/') {
            if (fp.get(next)) {
                if (next == '/') {
                    while (fp.get(ch) && ch != '\n');
                    continue;
                }
                if (next == '*') {
                    while (fp.get(ch)) {
                        if (ch == '*' && fp.get(next)) {
                            if (next == '/')
                                break;
                            fp.unget();
                        }
                    }
                    continue;
                }
                fp.unget();
            }
            cout << "Operator: /\n";
            continue;
        }

        if (isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
            string token;
            do {
                if (token.length() < MAX_LEN)
                    token += ch;
            } while (fp.get(ch) && (isalnum(static_cast<unsigned char>(ch)) || ch == '_'));

            if (fp)
                fp.unget();

            cout << (isKeyword(token) ? "Keyword: " : "Identifier: ") << token << "\n";
        } else if (isdigit(static_cast<unsigned char>(ch))) {
            string token;
            do {
                token += ch;
            } while (fp.get(ch) && (isdigit(static_cast<unsigned char>(ch)) || ch == '.'));

            if (fp)
                fp.unget();

            cout << "Number: " << token << "\n";
        } else if (operators.find(ch) != string::npos) {
            if (fp.get(next)) {
                if ((ch == '+' && next == '+') || (ch == '-' && next == '-') ||  ((ch == '=' || ch == '<' || ch == '>') && next == '=')) {
                    cout << "Operator: " << ch << next << "\n";
                } else {
                    cout << "Operator: " << ch << "\n";
                    fp.unget();
                }
            } else {
                cout << "Operator: " << ch << "\n";
            }
        } else if (symbols.find(ch) != string::npos) {
            cout << "Symbol: " << ch << "\n";
        } else {
            cout << "Unknown: " << ch << "\n";
        }
    }

    fp.close();
    return 0;
}