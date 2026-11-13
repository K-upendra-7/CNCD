
#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
set<char> visited;
char start;

set<char> FIRST(char ch){
    if (!grammar.count(ch))
        return {ch};

    if (visited.count(ch))
        return {};

    visited.insert(ch);
    set<char> result;

    for (string p : grammar[ch])    {
        if (p == "#")        {
            result.insert('#');
            continue;
        }

        for (char c : p)        {
            set<char> temp = FIRST(c);

            for (char x : temp)
                if (x != '#')
                    result.insert(x);

            if (!temp.count('#'))
                break;
        }

        if (p.size() > 0)        {
            bool nullable = true;

            for (char c : p)
                if (!FIRST(c).count('#')){
                    nullable = false;
                    break;
                }

            if (nullable)
                result.insert('#');
        }
    }

    visited.erase(ch);
    return result;
}

set<char> FOLLOW(char ch){
    set<char> result;

    if (ch == start)
        result.insert('$');

    for (auto [lhs, productions] : grammar)    {
        for (string p : productions)        {
            for (int i = 0; i < p.size(); i++)            {
                if (p[i] == ch)                {
                    if (i + 1 < p.size())                    {
                        bool nullable = true;

                        for (int j = i + 1; j < p.size(); j++)                        {
                            set<char> temp = FIRST(p[j]);

                            for (char x : temp)
                                if (x != '#')
                                    result.insert(x);

                            if (!temp.count('#'))                            {
                                nullable = false;
                                break;
                            }
                        }

                        if (nullable && lhs != ch)                        {
                            set<char> temp = FOLLOW(lhs);
                            result.insert(temp.begin(), temp.end());
                        }
                    }
                    else if (lhs != ch)                    {
                        set<char> temp = FOLLOW(lhs);
                        result.insert(temp.begin(), temp.end());
                    }
                }
            }
        }
    }

    return result;
}

int main()
{
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (A->aB):\n";

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        char lhs = s[0];
        start = (i == 0) ? lhs : start;

        string rhs = s.substr(3);
        stringstream ss(rhs);
        string p;

        while (getline(ss, p, '|'))
            grammar[lhs].push_back(p);
    }

    cout << "First:";
    for (auto [nt, productions] : grammar) {
        visited.clear();
        firstSet[nt] = FIRST(nt);

        cout << "FIRST(" << nt << ") = { ";
        for (char c : firstSet[nt])
            cout << c << " ";
        cout << "}\n";
    }
    
    cout << "Follow:";
    for (auto [nt, productions] : grammar) {
        followSet[nt] = FOLLOW(nt);

        cout << "FOLLOW(" << nt << ") = { ";
        for (char c : followSet[nt])
            cout << c << " ";
        cout << "}\n";
    }

    return 0;
}
