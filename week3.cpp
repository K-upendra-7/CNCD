#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
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
    cout << "Enter productions (e.g., A->aB|c):\n";

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

    cout << "\nFIRST Sets:\n";
    for (auto& [nt, s] : firstSet) {
        cout << "FIRST(" << nt << ") = { ";
        for (char c : s) cout << c << " ";
        cout << "}\n";
    }

    cout << "\nFOLLOW Sets:\n";
    for (auto& [nt, s] : followSet) {
        cout << "FOLLOW(" << nt << ") = { ";
        for (char c : s) cout << c << " ";
        cout << "}\n";
    }

    return 0;
}