#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> FIRST, FOLLOW;
map<pair<char, char>, string> table;
set<char> nonTerminals;

set<char> firstOfString(const string& s);

void findFirst(char c) {
    if (!isupper(c)) {
        FIRST[c].insert(c);
        return;
    }

    for (string prod : grammar[c]) {
        if (prod == "#") {
            FIRST[c].insert('#');
            continue;
        }

        bool allEpsilon = true;

        for (char x : prod) {
            if (!isupper(x)) {
                FIRST[c].insert(x);
                allEpsilon = false;
                break;
            }

            if (FIRST[x].empty())
                findFirst(x);

            for (char f : FIRST[x]) {
                if (f != '#')
                    FIRST[c].insert(f);
            }

            if (!FIRST[x].count('#')) {
                allEpsilon = false;
                break;
            }
        }

        if (allEpsilon)
            FIRST[c].insert('#');
    }
}

set<char> firstOfString(const string& s) {
    set<char> result;

    if (s.empty()) {
        result.insert('#');
        return result;
    }

    bool allEpsilon = true;

    for (char c : s) {
        if (!isupper(c)) {
            result.insert(c);
            allEpsilon = false;
            break;
        }

        for (char x : FIRST[c]) {
            if (x != '#')
                result.insert(x);
        }

        if (!FIRST[c].count('#')) {
            allEpsilon = false;
            break;
        }
    }

    if (allEpsilon)
        result.insert('#');

    return result;
}

void findFollow(char start) {
    FOLLOW[start].insert('$');

    bool changed = true;

    while (changed) {
        changed = false;

        for (auto &[lhs, productions] : grammar) {
            for (string prod : productions) {

                for (int i = 0; i < (int)prod.size(); i++) {

                    char B = prod[i];

                    if (!isupper(B))
                        continue;

                    string beta = prod.substr(i + 1);

                    set<char> firstBeta = firstOfString(beta);

                    for (char x : firstBeta) {
                        if (x != '#') {
                            if (FOLLOW[B].insert(x).second)
                                changed = true;
                        }
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

void createParsingTable() {

    for (auto &[lhs, productions] : grammar) {

        for (string prod : productions) {

            set<char> first = firstOfString(prod);

            for (char terminal : first) {

                if (terminal != '#')
                    table[{lhs, terminal}] = prod;
            }

            if (first.count('#')) {

                for (char terminal : FOLLOW[lhs]) {
                    table[{lhs, terminal}] = prod;
                }
            }
        }
    }
}

void printFirst() {

    cout << "\nFIRST SETS\n";
    cout << "-------------------------\n";

    for (char nt : nonTerminals) {

        cout << "FIRST(" << nt << ") = { ";

        for (char x : FIRST[nt])
            cout << x << " ";

        cout << "}\n";
    }
}

void printFollow() {

    cout << "\nFOLLOW SETS\n";
    cout << "-------------------------\n";

    for (char nt : nonTerminals) {

        cout << "FOLLOW(" << nt << ") = { ";

        for (char x : FOLLOW[nt])
            cout << x << " ";

        cout << "}\n";
    }
}

void printParsingTable() {

    vector<char> terminals;

    set<char> terminalSet;

    for (auto &[lhs, productions] : grammar) {

        for (string prod : productions) {

            for (char c : prod) {

                if (!isupper(c) && c != '#')
                    terminalSet.insert(c);
            }
        }
    }

    for (char nt : nonTerminals) {

        for (char c : FOLLOW[nt])
            terminalSet.insert(c);
    }

    for (char c : terminalSet)
        terminals.push_back(c);

    cout << "\nPREDICTIVE PARSING TABLE\n";
    cout << "------------------------------------------------\n";

    cout << setw(10) << " ";

    for (char t : terminals)
        cout << setw(10) << t;

    cout << "\n";

    cout << string(10 + terminals.size() * 10, '-') << "\n";

    for (char nt : nonTerminals) {

        cout << setw(10) << nt;

        for (char t : terminals) {

            if (table.count({nt, t}))
                cout << setw(10) << table[{nt, t}];
            else
                cout << setw(10) << "-";
        }

        cout << "\n";
    }
}

void parseString(string input, char start) {

    input += '$';

    stack<char> st;

    st.push('$');
    st.push(start);

    int pos = 0;

    cout << "\nPARSING STRING\n";
    cout << "-------------------------------------------------------------\n";

    cout << left
         << setw(25) << "Stack"
         << setw(25) << "Input"
         << "Action\n";

    cout << "-------------------------------------------------------------\n";

    while (!st.empty()) {

        char top = st.top();
        char current = input[pos];

        string stackString;

        stack<char> temp = st;

        while (!temp.empty()) {
            stackString += temp.top();
            temp.pop();
        }

        string remaining = input.substr(pos);

        cout << left
             << setw(25) << stackString
             << setw(25) << remaining;

        if (top == '$' && current == '$') {

            cout << "ACCEPT\n";

            cout << "\nString accepted.\n";

            return;
        }

        if (!isupper(top)) {

            if (top == current) {

                st.pop();
                pos++;

                cout << "Match " << current << "\n";
            }
            else {

                cout << "ERROR\n";

                cout << "\nString rejected.\n";

                return;
            }
        }
        else {

            if (!table.count({top, current})) {

                cout << "ERROR\n";

                cout << "\nString rejected.\n";

                return;
            }

            string production = table[{top, current}];

            st.pop();

            if (production == "#") {

                cout << top << " -> epsilon\n";
            }
            else {

                cout << top << " -> " << production << "\n";

                for (int i = production.size() - 1; i >= 0; i--)
                    st.push(production[i]);
            }
        }
    }
}

int main() {

    int n;

    cout << "Enter number of productions: ";
    cin >> n;

    cout << "\nEnter productions in the format:\n";
    cout << "E->TA\n";
    cout << "A->+TA|#\n";
    cout << "Use # for epsilon.\n\n";

    string production;

    char startSymbol;

    for (int i = 0; i < n; i++) {

        cin >> production;

        char lhs = production[0];

        if (i == 0)
            startSymbol = lhs;

        nonTerminals.insert(lhs);

        string rhs = production.substr(3);

        string current;

        for (char c : rhs) {

            if (c == '|') {

                grammar[lhs].push_back(current);

                current.clear();
            }
            else {
                current += c;
            }
        }

        if (!current.empty())
            grammar[lhs].push_back(current);
    }

    for (char nt : nonTerminals)
        findFirst(nt);

    findFollow(startSymbol);

    createParsingTable();

    printFirst();

    printFollow();

    printParsingTable();

    string input;

    cout << "\nEnter string to parse: ";
    cin >> input;

    parseString(input, startSymbol);

    return 0;
}

// Example run

// Enter this grammar:

// 5
// E->TA
// A->+TA|#
// T->FB
// B->*FB|#
// F->(E)|i

// The program prints:

// FIRST SETS
// -------------------------
// FIRST(A) = { # + }
// FIRST(B) = { # * }
// FIRST(E) = { ( i }
// FIRST(F) = { ( i }
// FIRST(T) = { ( i }

// FOLLOW SETS
// -------------------------
// FOLLOW(A) = { $ ) }
// FOLLOW(B) = { $ ) + }
// FOLLOW(E) = { $ ) }
// FOLLOW(F) = { $ ) * + }
// FOLLOW(T) = { $ ) + }

// Then:

// PREDICTIVE PARSING TABLE
// ------------------------------------------------
//                    (         )         *         +         i         $
// --------------------------------------------------------------------
//          E        TA         -         -         -        TA         -
//          A         -         #         -       +TA         -         #
//          T        FB         -         -         -        FB         -
//          B         -         #       *FB         #         -         #
//          F       (E)         -         -         -         i         -

// Then enter:

// i+i*i

// The parser will display the stack, remaining input, and action at every step, ending with:

// String accepted.