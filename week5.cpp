#include <bits/stdc++.h>
using namespace std;

vector<pair<char, string>> grammar;
set<char> nonterms, terms, symbols;
map<char, set<char>> firstSet;
map<pair<int, char>, string> action;
map<pair<int, char>, int> goTo;

using Item = tuple<int, int, char>;

set<char> firstOfString(string s) {
    if (s.empty()) return {'#'};
    set<char> res;
    for (char c : s) {
        set<char> temp = nonterms.count(c) ? firstSet[c] : set<char>{c};
        for (char x : temp) if (x != '#') res.insert(x);
        if (!temp.count('#')) return res;
    }
    res.insert('#');
    return res;
}

set<Item> closure(set<Item> items) {
    for (bool chg = true; chg; ) {
        chg = false;
        set<Item> next = items;
        for (auto [p, dot, la] : items) {
            string& rhs = grammar[p].second;
            if (dot >= rhs.size() || !nonterms.count(rhs[dot])) continue;
            for (char b : firstOfString(rhs.substr(dot + 1) + la))
                if (b != '#')
                    for (int i = 1; i < grammar.size(); i++)
                        if (grammar[i].first == rhs[dot])
                            chg |= next.insert({i, 0, b}).second;
        }
        items = next;
    }
    return items;
}

set<Item> gotoState(const set<Item>& items, char sym) {
    set<Item> res;
    for (auto [p, dot, la] : items)
        if (dot < grammar[p].second.size() && grammar[p].second[dot] == sym)
            res.insert({p, dot + 1, la});
    return res.empty() ? res : closure(res);
}

bool addAction(int s, char sym, string val) {
    if (action.count({s, sym}) && action[{s, sym}] != val) return false;
    action[{s, sym}] = val;
    return true;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cout << "Enter productions (S->CC):\n";

    for (int i = 0; i < n; i++) {
        string s, p;
        cin >> s;
        char lhs = s[0];
        nonterms.insert(lhs);
        if (i == 0) grammar.push_back({'@', string(1, lhs)}); // '@' represents S'
        stringstream ss(s.substr(3));
        while (getline(ss, p, '|'))
            grammar.push_back({lhs, p == "#" ? "" : p});
    }

    for (auto& [lhs, rhs] : grammar)
        for (char c : rhs)
            if (!nonterms.count(c)) terms.insert(c);

    symbols = terms;
    symbols.insert(nonterms.begin(), nonterms.end());

    // Calculate FIRST
    for (bool chg = true; chg; ) {
        chg = false;
        for (int i = 1; i < grammar.size(); i++)
            for (char c : firstOfString(grammar[i].second))
                chg |= firstSet[grammar[i].first].insert(c).second;
    }

    // Build Canonical LR(1) Collection
    vector<set<Item>> states = {closure({{0, 0, '$'}} )};
    map<pair<int, char>, int> trans;

    for (int i = 0; i < states.size(); i++) {
        for (char sym : symbols) {
            set<Item> nxt = gotoState(states[i], sym);
            if (nxt.empty()) continue;
            auto it = find(states.begin(), states.end(), nxt);
            int id = it - states.begin();
            if (it == states.end()) states.push_back(nxt);
            trans[{i, sym}] = id;
        }
    }

    // Merge LR(1) states with identical LR(0) cores into LALR(1) states
    map<set<pair<int, int>>, int> coreMap;
    vector<set<Item>> merged;
    vector<int> grp(states.size());

    for (int i = 0; i < states.size(); i++) {
        set<pair<int, int>> core;
        for (auto [p, dot, la] : states[i]) core.insert({p, dot});
        if (!coreMap.count(core)) {
            coreMap[core] = merged.size();
            merged.push_back({});
        }
        grp[i] = coreMap[core];
        merged[grp[i]].insert(states[i].begin(), states[i].end());
    }

    map<pair<int, char>, int> newTrans;
    for (auto [k, v] : trans) newTrans[{grp[k.first], k.second}] = grp[v];
    states = merged;
    trans = newTrans;

    cout << "\nNumber of LALR States: " << states.size() << "\n";

    // Build ACTION and GOTO tables
    for (int i = 0; i < states.size(); i++) {
        for (auto [p, dot, la] : states[i]) {
            string& rhs = grammar[p].second;
            if (dot < rhs.size()) {
                char ch = rhs[dot];
                if (terms.count(ch) && !addAction(i, ch, "S" + to_string(trans[{i, ch}])))
                    return cout << "\nGrammar is not LALR(1)\n", 0;
                if (nonterms.count(ch)) goTo[{i, ch}] = trans[{i, ch}];
            } else if (p == 0 && la == '$') {
                if (!addAction(i, '$', "ACC")) return cout << "\nGrammar is not LALR(1)\n", 0;
            } else if (!addAction(i, la, "R" + to_string(p))) {
                return cout << "\nGrammar is not LALR(1)\n", 0;
            }
        }
    }

    // Display Tables
    set<char> cols = terms;
    cols.insert('$');
    cout << "\nACTION TABLE\nState\t";
    for (char c : cols) cout << c << "\t";
    cout << "\n";
    for (int i = 0; i < states.size(); i++) {
        cout << i << "\t";
        for (char c : cols) cout << (action.count({i, c}) ? action[{i, c}] : "-") << "\t";
        cout << "\n";
    }

    cout << "\nGOTO TABLE\nState\t";
    for (char c : nonterms) cout << c << "\t";
    cout << "\n";
    for (int i = 0; i < states.size(); i++) {
        cout << i << "\t";
        for (char c : nonterms) cout << (goTo.count({i, c}) ? to_string(goTo[{i, c}]) : "-") << "\t";
        cout << "\n";
    }

    // Parse Input String
    string input;
    cout << "\nEnter input string: ";
    cin >> input;
    input += '$';

    vector<int> st = {0};
    int idx = 0;
    cout << "\nPARSER TRACE TABLE\n" << left << setw(25) << "STACK" << setw(20) << "INPUT" << "ACTION\n";

    while (true) {
        string stStr;
        for (int x : st) stStr += to_string(x) + " ";
        int curState = st.back();
        char curSym = input[idx];

        cout << left << setw(25) << stStr << setw(20) << input.substr(idx);
        if (!action.count({curState, curSym})) {
            cout << "ERROR\n\nString Rejected\n";
            break;
        }

        string op = action[{curState, curSym}];
        if (op == "ACC") {
            cout << "ACCEPT\n\nString Accepted\n";
            break;
        } else if (op[0] == 'S') {
            int nxt = stoi(op.substr(1));
            cout << "SHIFT " << nxt << "\n";
            st.push_back(nxt);
            idx++;
        } else {
            int p = stoi(op.substr(1));
            auto [lhs, rhs] = grammar[p];
            cout << "REDUCE " << lhs << "->" << (rhs.empty() ? "#" : rhs) << "\n";
            st.resize(st.size() - rhs.size());
            st.push_back(goTo[{st.back(), lhs}]);
        }
    }
    return 0;
}