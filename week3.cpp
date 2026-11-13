#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
char startSymbol;

set<char> firstOfString(string s) {
    if (s.empty() || s == "#") return {'#'};
    set<char> res;
    for (char c : s) {
        set<char> temp = grammar.count(c) ? firstSet[c] : set<char>{c};
        for (char x : temp) if (x != '#') res.insert(x);
        if (!temp.count('#')) return res;
    }
    res.insert('#');
    return res;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cout << "Enter productions (A->aB):\n";

    for (int i = 0; i < n; i++) {
        string s, p;
        cin >> s;
        char lhs = s[0];
        if (i == 0) startSymbol = lhs;
        stringstream ss(s.substr(3));
        while (getline(ss, p, '|')) grammar[lhs].push_back(p);
    }

    for (bool chg = true; chg; ) {
        chg = false;
        for (auto& [nt, prods] : grammar)
            for (string& p : prods)
                for (char c : firstOfString(p))
                    chg |= firstSet[nt].insert(c).second;
    }
    
    followSet[startSymbol].insert('$');
    for (bool chg = true; chg; ) {
        chg = false;
        for (auto& [lhs, prods] : grammar) {
            for (string& p : prods) {
                for (int i = 0; i < p.size(); i++) {
                    if (!grammar.count(p[i])) continue;
                    set<char> temp = firstOfString(p.substr(i + 1));
                    for (char c : temp)
                        if (c != '#') chg |= followSet[p[i]].insert(c).second;
                    if (temp.count('#'))
                        for (char c : followSet[lhs])
                            chg |= followSet[p[i]].insert(c).second;
                }
            }
        }
    }

    cout << "\nFirst:\n";
    for (auto& [nt, _] : grammar) {
        cout << "FIRST(" << nt << ") = { ";
        for (char c : firstSet[nt]) cout << c << " ";
        cout << "}\n";
    }

    cout << "\nFollow:\n";
    for (auto& [nt, _] : grammar) {
        cout << "FOLLOW(" << nt << ") = { ";
        for (char c : followSet[nt]) cout << c << " ";
        cout << "}\n";
    }
    return 0;
}