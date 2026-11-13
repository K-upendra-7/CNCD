#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<pair<char, char>, string> table;
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
    cout << "Enter productions (E->TR):\n";

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
        for (auto& [lhs, prods] : grammar)
            for (string& p : prods)
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

    auto printSets = [](string title, map<char, set<char>>& mp) {
        cout << "\n" << title << "\n";
        for (auto& [nt, s] : mp) {
            cout << nt << " : { ";
            for (char c : s) cout << c << " ";
            cout << "}\n";
        }
    };
    printSets("FIRST SETS", firstSet);
    printSets("FOLLOW SETS", followSet);

    set<char> terms = {'$'};
    for (auto& [nt, prods] : grammar) {
        for (string& p : prods) {
            set<char> temp = firstOfString(p);
            if (temp.count('#'))
                temp.insert(followSet[nt].begin(), followSet[nt].end());
            for (char t : temp) {
                if (t == '#') continue;
                if (table.count({nt, t})) return cout << "\nGrammar is not LL(1)\n", 0;
                table[{nt, t}] = p;
                terms.insert(t);
            }
        }
    }

    cout << "\nPREDICTIVE PARSING TABLE\nNT\t";
    for (char t : terms) cout << t << "\t";
    cout << "\n";
    for (auto& [nt, _] : grammar) {
        cout << nt << "\t";
        for (char t : terms) cout << (table.count({nt, t}) ? table[{nt, t}] : "-") << "\t";
        cout << "\n";
    }

    string input, st = {'$', startSymbol};
    cout << "\nEnter input string: ";
    cin >> input;
    input += '$';

    cout << "\nSTACK\t\tINPUT\t\tACTION\n";
    int i = 0;
    while (!st.empty()) {
        char top = st.back(), cur = input[i];
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
            if (p != "#") st.append(p.rbegin(), p.rend());
        } else {
            cout << "ERROR\n\nString Rejected\n";
            return 0;
        }
    }
    return 0;
}