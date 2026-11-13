#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> FIRST, FOLLOW;
set<char> nonTerminals;

set<char> firstOfString(const string& str);

void findFirst(char symbol) {
    if (!isupper(symbol)) {
        FIRST[symbol].insert(symbol);
        return;
    }

    for (string production : grammar[symbol]) {
        if (production == "#") {
            FIRST[symbol].insert('#');
            continue;
        }

        bool epsilon = true;

        for (char c : production) {
            if (!isupper(c)) {
                FIRST[symbol].insert(c);
                epsilon = false;
                break;
            }

            findFirst(c);

            for (char x : FIRST[c]) {
                if (x != '#')
                    FIRST[symbol].insert(x);
            }

            if (FIRST[c].count('#') == 0) {
                epsilon = false;
                break;
            }
        }

        if (epsilon)
            FIRST[symbol].insert('#');
    }
}

set<char> firstOfString(const string& str) {
    set<char> result;
    bool epsilon = true;

    for (char c : str) {
        if (!isupper(c)) {
            result.insert(c);
            epsilon = false;
            break;
        }

        for (char x : FIRST[c]) {
            if (x != '#')
                result.insert(x);
        }

        if (!FIRST[c].count('#')) {
            epsilon = false;
            break;
        }
    }

    if (epsilon)
        result.insert('#');

    return result;
}

void findFollow() {
    char start = *nonTerminals.begin();
    FOLLOW[start].insert('$');

    bool changed = true;

    while (changed) {
        changed = false;

        for (auto &[lhs, productions] : grammar) {
            for (string production : productions) {

                for (int i = 0; i < production.size(); i++) {
                    char B = production[i];

                    if (!isupper(B))
                        continue;

                    string beta = production.substr(i + 1);

                    set<char> firstBeta = firstOfString(beta);

                    for (char x : firstBeta) {
                        if (x != '#')
                            if (FOLLOW[B].insert(x).second)
                                changed = true;
                    }

                    if (beta.empty() || firstBeta.count('#')) {
                        for (char x : FOLLOW[lhs]) {
                            if (FOLLOW[B].insert(x).second)
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int n;

    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions in the form A->xyz\n";
    cout << "Use # for epsilon.\n\n";

    for (int i = 0; i < n; i++) {
        string production;
        cin >> production;

        char lhs = production[0];
        nonTerminals.insert(lhs);

        string rhs = production.substr(3);

        string current;

        for (char c : rhs) {
            if (c == '|') {
                grammar[lhs].push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }

        if (!current.empty())
            grammar[lhs].push_back(current);
    }

    for (char nt : nonTerminals)
        findFirst(nt);

    findFollow();

    cout << "\nFIRST sets:\n";

    for (char nt : nonTerminals) {
        cout << "FIRST(" << nt << ") = { ";

        for (char x : FIRST[nt])
            cout << x << " ";

        cout << "}\n";
    }

    cout << "\nFOLLOW sets:\n";

    for (char nt : nonTerminals) {
        cout << "FOLLOW(" << nt << ") = { ";

        for (char x : FOLLOW[nt])
            cout << x << " ";

        cout << "}\n";
    }

    return 0;
}

// Example

// Input:

// 5
// E->TA
// A->+TA|#
// T->FB
// B->*FB|#
// F->(E)|i

// Output:

// FIRST sets:
// FIRST(E) = { ( i }
// FIRST(A) = { # + }
// FIRST(T) = { ( i }
// FIRST(B) = { # * }
// FIRST(F) = { ( i }

// FOLLOW sets:
// FOLLOW(E) = { $ ) }
// FOLLOW(A) = { $ ) }
// FOLLOW(T) = { $ ) + }
// FOLLOW(B) = { $ ) + }
// FOLLOW(F) = { $ ) * + }