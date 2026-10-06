#include <bits/stdc++.h>
using namespace std;

struct Prod {
    char lhs;
    string rhs;
};

struct Item {
    int p, dot;
    char la;

    bool operator<(const Item& x) const {
        return tie(p, dot, la) < tie(x.p, x.dot, x.la);
    }

    bool operator==(const Item& x) const {
        return p == x.p && dot == x.dot && la == x.la;
    }
};

vector<Prod> P;
set<char> NT, T;
map<char, set<char>> FIRST;

set<char> firstString(string s) {
    set<char> res;
    bool eps = true;

    for (char x : s) {
        if (!eps) break;

        eps = false;

        if (T.count(x) || x == '$') {
            res.insert(x);
        } else {
            for (char c : FIRST[x])
                if (c != '#') res.insert(c);

            if (FIRST[x].count('#'))
                eps = true;
        }
    }

    if (eps) res.insert('#');
    return res;
}

void computeFirst() {
    for (char x : NT)
        FIRST[x] = {};

    bool change = true;

    while (change) {
        change = false;

        for (auto &pr : P) {
            set<char> f = firstString(pr.rhs);

            for (char x : f) {
                if (!FIRST[pr.lhs].count(x)) {
                    FIRST[pr.lhs].insert(x);
                    change = true;
                }
            }
        }
    }
}

set<Item> closure(set<Item> I) {
    bool change = true;

    while (change) {
        change = false;

        for (auto it : I) {
            Prod &pr = P[it.p];

            if (it.dot >= (int)pr.rhs.size())
                continue;

            char B = pr.rhs[it.dot];

            if (!NT.count(B))
                continue;

            string beta = pr.rhs.substr(it.dot + 1);
            beta += it.la;

            set<char> f = firstString(beta);

            for (int j = 0; j < (int)P.size(); j++) {
                if (P[j].lhs != B)
                    continue;

                for (char a : f) {
                    if (a == '#') continue;

                    Item ni{j, 0, a};

                    if (!I.count(ni)) {
                        I.insert(ni);
                        change = true;
                    }
                }
            }
        }
    }

    return I;
}

set<Item> goTo(set<Item> I, char X) {
    set<Item> J;

    for (auto it : I) {
        Prod &pr = P[it.p];

        if (it.dot < (int)pr.rhs.size() &&
            pr.rhs[it.dot] == X) {
            J.insert({it.p, it.dot + 1, it.la});
        }
    }

    if (J.empty()) return J;

    return closure(J);
}

string core(set<Item> I) {
    string s;

    vector<pair<int, int>> v;

    for (auto x : I)
        v.push_back({x.p, x.dot});

    sort(v.begin(), v.end());

    for (auto [p, d] : v)
        s += to_string(p) + "," + to_string(d) + ";";

    return s;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    string line;
    getline(cin, line);

    char start;

    for (int i = 0; i < n; i++) {
        getline(cin, line);

        char lhs = line[0];

        if (i == 0)
            start = lhs;

        NT.insert(lhs);

        string rhs = line.substr(3);
        string cur;

        for (char c : rhs) {
            if (c == '|') {
                if (cur == "#") cur = "";
                P.push_back({lhs, cur});
                cur = "";
            } else {
                cur += c;
            }
        }

        if (cur == "#") cur = "";
        P.push_back({lhs, cur});
    }

    for (auto &p : P) {
        for (char c : p.rhs) {
            if (!NT.count(c))
                T.insert(c);
        }
    }

    T.insert('$');

    char aug = '@';
    P.insert(P.begin(), {aug, string(1, start)});
    NT.insert(aug);

    computeFirst();

    vector<set<Item>> lr1;

    set<Item> I0;
    I0.insert({0, 0, '$'});
    I0 = closure(I0);

    lr1.push_back(I0);

    map<pair<int, char>, int> trans;

    for (int i = 0; i < (int)lr1.size(); i++) {
        set<char> symbols;

        for (auto it : lr1[i]) {
            Prod &p = P[it.p];

            if (it.dot < (int)p.rhs.size())
                symbols.insert(p.rhs[it.dot]);
        }

        for (char X : symbols) {
            set<Item> J = goTo(lr1[i], X);

            if (J.empty())
                continue;

            int id = -1;

            for (int k = 0; k < (int)lr1.size(); k++) {
                if (lr1[k] == J) {
                    id = k;
                    break;
                }
            }

            if (id == -1) {
                id = lr1.size();
                lr1.push_back(J);
            }

            trans[{i, X}] = id;
        }
    }

    map<string, int> mergedId;
    vector<set<Item>> lalr;
    vector<int> oldToNew(lr1.size());

    for (int i = 0; i < (int)lr1.size(); i++) {
        string c = core(lr1[i]);

        if (!mergedId.count(c)) {
            int id = lalr.size();
            mergedId[c] = id;
            lalr.push_back({});
        }

        int id = mergedId[c];
        oldToNew[i] = id;

        for (auto x : lr1[i])
            lalr[id].insert(x);
    }

    map<pair<int, char>, int> go;

    for (auto &[key, val] : trans) {
        int from = oldToNew[key.first];
        int to = oldToNew[val];

        go[{from, key.second}] = to;
    }

    map<pair<int, char>, string> ACTION;
    map<pair<int, char>, int> GOTO;

    bool conflict = false;

    auto addAction = [&](int s, char a, string act) {
        auto key = make_pair(s, a);

        if (ACTION.count(key) && ACTION[key] != act) {
            cout << "Conflict at state " << s
                 << ", symbol " << a << endl;
            cout << ACTION[key] << " / " << act << endl;
            conflict = true;
        } else {
            ACTION[key] = act;
        }
    };

    for (int i = 0; i < (int)lalr.size(); i++) {
        for (auto it : lalr[i]) {
            Prod &p = P[it.p];

            if (it.dot < (int)p.rhs.size()) {
                char X = p.rhs[it.dot];

                if (T.count(X)) {
                    int j = go[{i, X}];

                    addAction(i, X, "s" + to_string(j));
                } else if (NT.count(X) && X != aug) {
                    GOTO[{i, X}] = go[{i, X}];
                }
            } else {
                if (it.p == 0 && it.la == '$') {
                    addAction(i, '$', "acc");
                } else {
                    string r = "r" + to_string(it.p);

                    addAction(i, it.la, r);
                }
            }
        }
    }

    vector<char> terminals;

    for (char x : T)
        terminals.push_back(x);

    vector<char> nonterminals;

    for (char x : NT)
        if (x != aug)
            nonterminals.push_back(x);

    cout << "\nLALR(1) Parsing Table\n\n";

    cout << setw(8) << "State";

    for (char x : terminals)
        cout << setw(8) << x;

    for (char x : nonterminals)
        cout << setw(8) << x;

    cout << "\n";

    for (int i = 0; i < (int)lalr.size(); i++) {
        cout << setw(8) << i;

        for (char x : terminals) {
            string a = ACTION[{i, x}];

            if (a.empty())
                a = "-";

            cout << setw(8) << a;
        }

        for (char x : nonterminals) {
            auto it = GOTO.find({i, x});

            if (it == GOTO.end())
                cout << setw(8) << "-";
            else
                cout << setw(8) << it->second;
        }

        cout << "\n";
    }

    cout << "\nProductions:\n";

    for (int i = 1; i < (int)P.size(); i++) {
        cout << i << ": "
             << P[i].lhs << " -> "
             << (P[i].rhs.empty() ? "#" : P[i].rhs)
             << "\n";
    }

    if (conflict) {
        cout << "\nGrammar has conflicts.\n";
        return 0;
    }

    string input;
    cout << "\nEnter string: ";
    cin >> input;
    input += '$';

    vector<int> st;
    st.push_back(0);

    int ip = 0;

    cout << "\nParsing Steps\n\n";
    cout << left
         << setw(25) << "Stack"
         << setw(20) << "Input"
         << "Action\n";

    while (true) {
        int state = st.back();
        char a = input[ip];

        string act = ACTION[{state, a}];

        string stackStr;

        for (int x : st)
            stackStr += to_string(x) + " ";

        string remaining = input.substr(ip);

        cout << left
             << setw(25) << stackStr
             << setw(20) << remaining
             << act << "\n";

        if (act.empty()) {
            cout << "\nRejected\n";
            break;
        }

        if (act == "acc") {
            cout << "\nAccepted\n";
            break;
        }

        if (act[0] == 's') {
            int next = stoi(act.substr(1));

            st.push_back(next);
            ip++;
        } else if (act[0] == 'r') {
            int pno = stoi(act.substr(1));

            Prod &p = P[pno];

            for (int k = 0; k < (int)p.rhs.size(); k++)
                st.pop_back();

            int next = GOTO[{st.back(), p.lhs}];

            st.push_back(next);
        }
    }

    return 0;
}

// Example input

// Use this grammar:

// 5
// E->E+T
// E->T
// T->T*i
// T->i
// T->(E)

// Then enter:

// i+i*i

// The program will first generate and print the LALR(1) ACTION/GOTO table, then perform bottom-up parsing:

// LALR(1) PARSING TABLE

//    State       i       +       *       (       )       $       E       T
// -----------------------------------------------------------------------
//        0      s2               ...             ...             1       3
//        ...

// PARSING STEPS

// Stack                    Input               Action
// ----------------------------------------------------------------------
// 0                        i+i*i$              s2
// 0 2                      +i*i$               r4
// ...
                                                          
// String accepted.
// Input format

// The program expects productions in this form:

// A->xyz

// Multiple alternatives can also be entered together:

// A->xyz|abc

// Use # for ε.

// For example:

// 5
// E->E+T
// E->T
// T->T*i
// T->i
// T->(E)

// Then the next input is the string to be parsed:

// i+i*i

// This way, the grammar is the input, the LALR parsing table is generated and printed, and then the given string is parsed using that table.