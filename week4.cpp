
#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<pair<char, char>, string> table;
map<char, set<char>> firstSet, followSet;
char start;

set<char> FIRST(char ch)
{
    if (!grammar.count(ch))
        return {ch};

    return firstSet[ch];
}

set<char> FOLLOW(char ch)
{
    return followSet[ch];
}

set<char> firstOfString(string s)
{
    set<char> result;

    if (s.empty())
        return {'#'};

    for (char c : s)
    {
        set<char> temp = FIRST(c);

        for (char x : temp)
            if (x != '#')
                result.insert(x);

        if (!temp.count('#'))
            return result;
    }

    result.insert('#');
    return result;
}

void calculateFirst()
{
    bool changed = true;

    while (changed)
    {
        changed = false;

        for (auto [nt, productions] : grammar)
        {
            for (string p : productions)
            {
                set<char> temp = firstOfString(p);

                for (char c : temp)
                    if (firstSet[nt].insert(c).second)
                        changed = true;
            }
        }
    }
}

void calculateFollow()
{
    followSet[start].insert('$');

    bool changed = true;

    while (changed)
    {
        changed = false;

        for (auto [lhs, productions] : grammar)
        {
            for (string p : productions)
            {
                for (int i = 0; i < p.size(); i++)
                {
                    char ch = p[i];

                    if (!grammar.count(ch))
                        continue;

                    set<char> temp = firstOfString(p.substr(i + 1));

                    for (char c : temp)
                    {
                        if (c != '#' && followSet[ch].insert(c).second)
                            changed = true;
                    }

                    if (temp.count('#'))
                    {
                        for (char c : followSet[lhs])
                            if (followSet[ch].insert(c).second)
                                changed = true;
                    }
                }
            }
        }
    }
}

int main()
{
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (E->TR):\n";

    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;

        int pos = s.find("->");

        if (pos == string::npos || pos != 1)
        {
            cout << "Invalid production\n";
            return 0;
        }

        char lhs = s[0];
        start = (i == 0) ? lhs : start;

        string rhs = s.substr(pos + 2);
        stringstream ss(rhs);
        string p;

        while (getline(ss, p, '|'))
            grammar[lhs].push_back(p);
    }

    calculateFirst();
    calculateFollow();

    cout << "\nFIRST SETS\n";

    for (auto [nt, productions] : grammar)
    {
        cout << "FIRST(" << nt << ") = { ";

        for (char c : FIRST(nt))
            cout << c << " ";

        cout << "}\n";
    }

    cout << "\nFOLLOW SETS\n";

    for (auto [nt, productions] : grammar)
    {
        cout << "FOLLOW(" << nt << ") = { ";

        for (char c : FOLLOW(nt))
            cout << c << " ";

        cout << "}\n";
    }

    set<char> terminals;

    for (auto [nt, productions] : grammar)
    {
        for (string p : productions)
        {
            set<char> temp = firstOfString(p);

            for (char t : temp)
            {
                if (t != '#')
                {
                    if (table.count({nt, t}))
                    {
                        cout << "\nGrammar is not LL(1)\n";
                        return 0;
                    }

                    table[{nt, t}] = p;
                    terminals.insert(t);
                }
            }

            if (temp.count('#'))
            {
                for (char t : FOLLOW(nt))
                {
                    if (table.count({nt, t}))
                    {
                        cout << "\nGrammar is not LL(1)\n";
                        return 0;
                    }

                    table[{nt, t}] = p;
                    terminals.insert(t);
                }
            }
        }
    }

    terminals.insert('$');

    cout << "\nPREDICTIVE PARSING TABLE\n";
    cout << "NT\t";

    for (char t : terminals)
        cout << t << "\t";

    cout << "\n";

    for (auto [nt, productions] : grammar)
    {
        cout << nt << "\t";

        for (char t : terminals)
        {
            if (table.count({nt, t}))
                cout << table[{nt, t}] << "\t";
            else
                cout << "-\t";
        }

        cout << "\n";
    }

    string input;
    cout << "\nEnter input string: ";
    cin >> input;
    input += '$';

    stack<char> st;
    st.push('$');
    st.push(start);

    int i = 0;
    bool accepted = false;

    cout << "\nSTACK\t\tINPUT\t\tACTION\n";

    while (!st.empty())
    {
        string currentStack = "";
        stack<char> temp = st;

        while (!temp.empty())
        {
            currentStack += temp.top();
            temp.pop();
        }

        string remaining = input.substr(i);
        char top = st.top();
        char current = input[i];

        cout << currentStack << "\t\t" << remaining << "\t\t";

        if (top == '$' && current == '$')
        {
            cout << "ACCEPT\n";
            accepted = true;
            break;
        }

        if (top == current)
        {
            cout << "Match " << current << "\n";
            st.pop();
            i++;
        }
        else if (grammar.count(top))
        {
            if (!table.count({top, current}))
            {
                cout << "ERROR\n";
                break;
            }

            string p = table[{top, current}];
            cout << top << "->" << p << "\n";

            st.pop();

            if (p != "#")
                for (int j = p.size() - 1; j >= 0; j--)
                    st.push(p[j]);
        }
        else
        {
            cout << "ERROR\n";
            break;
        }
    }

    if (accepted)
        cout << "\nString Accepted\n";
    else
        cout << "\nString Rejected\n";

    return 0;
}
