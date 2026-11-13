
#include <bits/stdc++.h>
using namespace std;

vector<pair<string, string>> grammar;
set<char> nonterminals, terminals;
map<char, set<char>> firstSet;
map<pair<int, char>, string> action;
map<pair<int, char>, int> goTo;

using Item = tuple<int, int, char>;

set<char> firstOfString(string s)
{
    set<char> result;

    if (s.empty())
        return {'#'};

    for (char ch : s)
    {
        set<char> temp;

        if (nonterminals.count(ch))
            temp = firstSet[ch];
        else
            temp.insert(ch);

        for (char c : temp)
            if (c != '#')
                result.insert(c);

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

        for (int i = 1; i < grammar.size(); i++)
        {
            string lhs = grammar[i].first;
            string rhs = grammar[i].second;

            set<char> temp = firstOfString(rhs);

            for (char ch : temp)
                if (firstSet[lhs[0]].insert(ch).second)
                    changed = true;
        }
    }
}

set<Item> closure(set<Item> items)
{
    set<Item> result = items;
    queue<Item> q;

    for (auto item : items)
        q.push(item);

    while (!q.empty())
    {
        auto [p, dot, lookahead] = q.front();
        q.pop();

        string rhs = grammar[p].second;

        if (dot >= rhs.size())
            continue;

        char ch = rhs[dot];

        if (!nonterminals.count(ch))
            continue;

        string beta = rhs.substr(dot + 1) + lookahead;
        set<char> lookaheads = firstOfString(beta);

        for (int i = 1; i < grammar.size(); i++)
        {
            if (grammar[i].first[0] != ch)
                continue;

            for (char la : lookaheads)
            {
                if (la == '#')
                    continue;

                Item item = {i, 0, la};

                if (result.insert(item).second)
                    q.push(item);
            }
        }
    }

    return result;
}

set<Item> gotoState(set<Item> items, char symbol)
{
    set<Item> result;

    for (auto [p, dot, la] : items)
    {
        string rhs = grammar[p].second;

        if (dot < rhs.size() && rhs[dot] == symbol)
            result.insert({p, dot + 1, la});
    }

    if (result.empty())
        return result;

    return closure(result);
}

void canonicalCollection(
    vector<set<Item>>& states,
    map<pair<int, char>, int>& transitions)
{
    set<Item> initial = closure({{0, 0, '$'}});

    states.push_back(initial);

    map<set<Item>, int> visited;
    visited[initial] = 0;

    queue<int> q;
    q.push(0);

    set<char> symbols = terminals;
    symbols.insert(nonterminals.begin(), nonterminals.end());

    while (!q.empty())
    {
        int i = q.front();
        q.pop();

        for (char ch : symbols)
        {
            set<Item> next = gotoState(states[i], ch);

            if (next.empty())
                continue;

            if (!visited.count(next))
            {
                int id = states.size();

                visited[next] = id;
                states.push_back(next);
                q.push(id);
            }

            transitions[{i, ch}] = visited[next];
        }
    }
}

void mergeStates(
    vector<set<Item>>& states,
    map<pair<int, char>, int>& transitions)
{
    map<set<pair<int, int>>, int> groups;
    vector<set<Item>> merged;
    vector<int> group(states.size());

    for (int i = 0; i < states.size(); i++)
    {
        set<pair<int, int>> core;

        for (auto [p, dot, la] : states[i])
            core.insert({p, dot});

        if (!groups.count(core))
        {
            groups[core] = merged.size();
            merged.push_back({});
        }

        group[i] = groups[core];

        merged[group[i]].insert(
            states[i].begin(), states[i].end());
    }

    map<pair<int, char>, int> newTransitions;

    for (auto [key, value] : transitions)
    {
        int a = group[key.first];
        int b = group[value];
        char ch = key.second;

        newTransitions[{a, ch}] = b;
    }

    states = merged;
    transitions = newTransitions;
}

bool addAction(int state, char symbol, string value)
{
    pair<int, char> key = {state, symbol};

    if (action.count(key) && action[key] != value)
        return false;

    action[key] = value;
    return true;
}

bool makeTable(
    vector<set<Item>>& states,
    map<pair<int, char>, int>& transitions)
{
    for (int i = 0; i < states.size(); i++)
    {
        for (auto [p, dot, la] : states[i])
        {
            string lhs = grammar[p].first;
            string rhs = grammar[p].second;

            if (dot < rhs.size())
            {
                char ch = rhs[dot];

                if (terminals.count(ch))
                {
                    int next = transitions[{i, ch}];

                    if (!addAction(i, ch, "S" + to_string(next)))
                        return false;
                }
                else
                {
                    goTo[{i, ch}] = transitions[{i, ch}];
                }
            }
            else if (p == 0 && la == '$')
            {
                if (!addAction(i, '$', "ACC"))
                    return false;
            }
            else
            {
                if (!addAction(i, la, "R" + to_string(p)))
                    return false;
            }
        }
    }

    return true;
}

void displayTable(int n)
{
    cout << "\nACTION TABLE\n";
    cout << "State\t";

    set<char> cols = terminals;
    cols.insert('$');

    for (char ch : cols)
        cout << ch << "\t";

    cout << "\n";

    for (int i = 0; i < n; i++)
    {
        cout << i << "\t";

        for (char ch : cols)
        {
            if (action.count({i, ch}))
                cout << action[{i, ch}] << "\t";
            else
                cout << "-\t";
        }

        cout << "\n";
    }

    cout << "\nGOTO TABLE\n";
    cout << "State\t";

    for (char ch : nonterminals)
        cout << ch << "\t";

    cout << "\n";

    for (int i = 0; i < n; i++)
    {
        cout << i << "\t";

        for (char ch : nonterminals)
        {
            if (goTo.count({i, ch}))
                cout << goTo[{i, ch}] << "\t";
            else
                cout << "-\t";
        }

        cout << "\n";
    }
}

void parseString(string input)
{
    input += '$';

    vector<int> st = {0};
    int i = 0;
    bool accepted = false;

    cout << "\nPARSER TRACE TABLE\n";
    cout << left << setw(25) << "STACK"
         << setw(20) << "INPUT"
         << "ACTION\n";

    while (true)
    {
        string stackStr;

        for (int x : st)
            stackStr += to_string(x) + " ";

        string remaining = input.substr(i);
        int state = st.back();

        char current = input[i];

        cout << left << setw(25) << stackStr
             << setw(20) << remaining;

        if (!action.count({state, current}))
        {
            cout << "ERROR\n";
            break;
        }

        string op = action[{state, current}];

        if (op == "ACC")
        {
            cout << "ACCEPT\n";
            accepted = true;
            break;
        }

        if (op[0] == 'S')
        {
            int next = stoi(op.substr(1));

            cout << "SHIFT " << next << "\n";

            st.push_back(next);
            i++;
        }
        else if (op[0] == 'R')
        {
            int p = stoi(op.substr(1));

            string lhs = grammar[p].first;
            string rhs = grammar[p].second;

            cout << "REDUCE " << lhs << "->"
                 << (rhs.empty() ? "#" : rhs) << "\n";

            if (!rhs.empty())
                st.resize(st.size() - rhs.size());

            if (!goTo.count({st.back(), lhs[0]}))
                break;

            st.push_back(goTo[{st.back(), lhs[0]}]);
        }
    }

    if (accepted)
        cout << "\nString Accepted\n";
    else
        cout << "\nString Rejected\n";
}

int main()
{
    int n;

    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (S->CC):\n";

    vector<pair<char, string>> productions;

    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;

        int pos = s.find("->");

        if (pos != 1 || pos == string::npos)
        {
            cout << "Invalid production\n";
            return 0;
        }

        char lhs = s[0];
        nonterminals.insert(lhs);

        string rhs = s.substr(pos + 2);
        stringstream ss(rhs);
        string p;

        while (getline(ss, p, '|'))
        {
            if (p == "#")
                p = "";

            productions.push_back({lhs, p});
        }
    }

    char start = productions[0].first;

    grammar.push_back({"S'", string(1, start)});

    for (auto [lhs, rhs] : productions)
        grammar.push_back({string(1, lhs), rhs});

    for (auto [lhs, rhs] : productions)
    {
        for (char ch : rhs)
        {
            if (!nonterminals.count(ch))
                terminals.insert(ch);
        }
    }

    calculateFirst();

    vector<set<Item>> states;
    map<pair<int, char>, int> transitions;

    canonicalCollection(states, transitions);

    mergeStates(states, transitions);

    cout << "\nNumber of LALR States: " << states.size() << "\n";

    if (!makeTable(states, transitions))
    {
        cout << "\nGrammar is not LALR(1)\n";
        return 0;
    }

    displayTable(states.size());

    string input;

    cout << "\nEnter input string: ";
    cin >> input;

    parseString(input);

    return 0;
}
