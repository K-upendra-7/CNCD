#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
map<pair<char, char>, string> table;
char startSymbol;

bool addSymbol(set<char>& s, char val) {
    return s.insert(val).second;
}

set<char> findFirst(string s) {
    if (s.empty() || s == "#") return {'#'};
    set<char> result;
    for (char c : s) {
        if (!grammar.count(c)) {
            result.insert(c);
            return result;
        }
        for (char x : firstSet[c]) {
            if (x != '#') result.insert(x);
        }
        if (!firstSet[c].count('#')) return result;
    }
    result.insert('#');
    return result;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cout << "Enter productions (e.g., E->TR):\n";

    for (int i = 0; i < n; i++) {
        string s, prod;
        cin >> s;
        char lhs = s[0];
        if (i == 0) startSymbol = lhs;

        stringstream ss(s.substr(3));
        while (getline(ss, prod, '|')) {
            grammar[lhs].push_back(prod);
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& [nt, prods] : grammar) {
            for (string p : prods) {
                for (char c : findFirst(p)) {
                    if (addSymbol(firstSet[nt], c)) changed = true;
                }
            }
        }
    }

    followSet[startSymbol].insert('$');
    changed = true;
    while (changed) {
        changed = false;
        for (auto& [lhs, prods] : grammar) {
            for (string p : prods) {
                for (int i = 0; i < p.size(); i++) {
                    char B = p[i];
                    if (!grammar.count(B)) continue;

                    set<char> nextFirst = findFirst(p.substr(i + 1));
                    for (char c : nextFirst) {
                        if (c != '#' && addSymbol(followSet[B], c)) changed = true;
                    }
                    if (nextFirst.count('#')) {
                        for (char c : followSet[lhs]) {
                            if (addSymbol(followSet[B], c)) changed = true;
                        }
                    }
                }
            }
        }
    }

    set<char> terminals = {'$'};
    for (auto& [nt, prods] : grammar) {
        for (string p : prods) {
            set<char> firstP = findFirst(p);

            if (firstP.count('#')) {
                for (char c : followSet[nt]) firstP.insert(c);
            }

            for (char t : firstP) {
                if (t == '#') continue;
                if (table.count({nt, t})) {
                    cout << "\nGrammar is not LL(1)\n";
                    return 0;
                }
                table[{nt, t}] = p;
                terminals.insert(t);
            }
        }
    }

    cout << "\nPREDICTIVE PARSING TABLE\nNT\t";
    for (char t : terminals) cout << t << "\t";
    cout << "\n";
    for (auto& [nt, _] : grammar) {
        cout << nt << "\t";
        for (char t : terminals) {
            cout << (table.count({nt, t}) ? table[{nt, t}] : "-") << "\t";
        }
        cout << "\n";
    }

    string input;
    cout << "\nEnter input string: ";
    cin >> input;
    input += '$';

    string st = "$";
    st += startSymbol;

    cout << "\nSTACK\t\tINPUT\t\tACTION\n";
    int i = 0;
    while (!st.empty()) {
        char top = st.back();
        char cur = input[i];

        cout << string(st.rbegin(), st.rend()) << "\t\t" << input.substr(i) << "\t\t";

        if (top == '$' && cur == '$') {
            cout << "ACCEPT\n\nString Accepted\n";
            return 0;
        } else if (top == cur) {
            cout << "Match " << cur << "\n";
            st.pop_back();
            i++;
        } else if (table.count({top, cur})) {
            string p = table[{top, cur}];
            cout << top << "->" << p << "\n";
            st.pop_back();
            if (p != "#") {
                for (int j = p.size() - 1; j >= 0; j--) st += p[j];
            }
        } else {
            cout << "ERROR\n\nString Rejected\n";
            return 0;
        }
    }
    return 0;
}