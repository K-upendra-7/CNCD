#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> first, follow;
char start;

set<char> findFirst(string s) {
    set<char> result;

    if (s == "#" || s.empty()) {
        result.insert('#');
        return result;
    }

    for (char c : s) {
        if (!grammar.count(c)) {
            result.insert(c);
            return result;
        }

        for (char x : first[c]) {
            if (x != '#')
                result.insert(x);
        }

        if (!first[c].count('#'))
            return result;
    }

    result.insert('#');
    return result;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (A->aB):\n";

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        char lhs = s[0];

        if (i == 0)
            start = lhs;

        string rhs = s.substr(3);
        string p = "";

        for (char c : rhs) {
            if (c == '|') {
                grammar[lhs].push_back(p);
                p = "";
            } else {
                p += c;
            }
        }

        grammar[lhs].push_back(p);
    }

    // Calculate FIRST
    bool change = true;

    while (change) {
        change = false;

        for (auto x : grammar) {
            char nt = x.first;

            for (string p : x.second) {
                set<char> temp = findFirst(p);

                for (char c : temp) {
                    if (first[nt].insert(c).second)
                        change = true;
                }
            }
        }
    }

    // Calculate FOLLOW
    follow[start].insert('$');

    change = true;

    while (change) {
        change = false;

        for (auto x : grammar) {
            char lhs = x.first;

            for (string p : x.second) {
                for (int i = 0; i < p.size(); i++) {

                    char c = p[i];

                    if (!grammar.count(c))
                        continue;

                    string remaining = p.substr(i + 1);
                    set<char> temp = findFirst(remaining);

                    for (char a : temp) {
                        if (a != '#') {
                            if (follow[c].insert(a).second)
                                change = true;
                        }
                    }

                    if (temp.count('#')) {
                        for (char a : follow[lhs]) {
                            if (follow[c].insert(a).second)
                                change = true;
                        }
                    }
                }
            }
        }
    }

    // Print FIRST
    cout << "\nFirst:\n";

    for (auto x : grammar) {
        char nt = x.first;

        cout << "FIRST(" << nt << ") = { ";
        for (char c : first[nt])
            cout << c << " ";
        cout << "}\n";
    }

    // Print FOLLOW
    cout << "\nFollow:\n";

    for (auto x : grammar) {
        char nt = x.first;

        cout << "FOLLOW(" << nt << ") = { ";
        for (char c : follow[nt])
            cout << c << " ";
        cout << "}\n";
    }

    return 0;
}
