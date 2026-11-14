#include <bits/stdc++.h>
using namespace std;

struct Item {
    int prod;
    int dot;
    char lookahead;

    bool operator<(const Item& other) const {
        return tie(prod, dot, lookahead) < tie(other.prod, other.dot, other.lookahead);
    }
    bool operator==(const Item& other) const {
        return tie(prod, dot, lookahead) == tie(other.prod, other.dot, other.lookahead);
    }
};

vector<pair<char, string>> grammar;
set<char> nonterms, terms, allSymbols;
map<char, set<char>> firstSet;
map<pair<int, char>, string> actionTable;
map<pair<int, char>, int> gotoTable;

set<char> findFirst(string s) {
    if (s.empty()) return {'#'};
    set<char> result;
    for (char c : s) {
        if (!nonterms.count(c)) {
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

set<Item> closure(set<Item> items) {
    bool changed = true;
    while (changed) {
        changed = false;
        set<Item> nextItems = items;

        for (Item it : items) {
            string rhs = grammar[it.prod].second;
            if (it.dot >= rhs.size()) continue;

            char B = rhs[it.dot];
            if (!nonterms.count(B)) continue;

            string beta = rhs.substr(it.dot + 1) + it.lookahead;
            set<char> lookaheads = findFirst(beta);

            for (int i = 1; i < grammar.size(); i++) {
                if (grammar[i].first == B) {
                    for (char b : lookaheads) {
                        if (b != '#' && nextItems.insert({i, 0, b}).second) {
                            changed = true;
                        }
                    }
                }
            }
        }
        items = nextItems;
    }
    return items;
}

set<Item> gotoState(const set<Item>& items, char sym) {
    set<Item> moved;
    for (Item it : items) {
        string rhs = grammar[it.prod].second;
        if (it.dot < rhs.size() && rhs[it.dot] == sym) {
            moved.insert({it.prod, it.dot + 1, it.lookahead});
        }
    }
    return moved.empty() ? moved : closure(moved);
}

bool addAction(int state, char sym, string val) {
    if (actionTable.count({state, sym}) && actionTable[{state, sym}] != val) {
        return false;
    }
    actionTable[{state, sym}] = val;
    return true;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cout << "Enter productions (e.g., S->CC):\n";

    for (int i = 0; i < n; i++) {
        string s, p;
        cin >> s;
        char lhs = s[0];
        nonterms.insert(lhs);

        if (i == 0) grammar.push_back({'@', string(1, lhs)});

        stringstream ss(s.substr(3));
        while (getline(ss, p, '|')) {
            grammar.push_back({lhs, p == "#" ? "" : p});
        }
    }

    for (auto& [lhs, rhs] : grammar) {
        for (char c : rhs) {
            if (!nonterms.count(c)) terms.insert(c);
        }
    }
    allSymbols = terms;
    allSymbols.insert(nonterms.begin(), nonterms.end());

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 1; i < grammar.size(); i++) {
            for (char c : findFirst(grammar[i].second)) {
                if (firstSet[grammar[i].first].insert(c).second) changed = true;
            }
        }
    }

    vector<set<Item>> states = {closure({{0, 0, '$'}} )};
    map<pair<int, char>, int> trans;

    for (int i = 0; i < states.size(); i++) {
        for (char sym : allSymbols) {
            set<Item> nextSt = gotoState(states[i], sym);
            if (nextSt.empty()) continue;

            auto it = find(states.begin(), states.end(), nextSt);
            int id = it - states.begin();
            if (it == states.end()) states.push_back(nextSt);
            trans[{i, sym}] = id;
        }
    }

    map<set<pair<int, int>>, int> coreToState;
    vector<set<Item>> lalrStates;
    vector<int> newId(states.size());

    for (int i = 0; i < states.size(); i++) {
        set<pair<int, int>> core;
        for (Item it : states[i]) core.insert({it.prod, it.dot});

        if (!coreToState.count(core)) {
            coreToState[core] = lalrStates.size();
            lalrStates.push_back({});
        }
        newId[i] = coreToState[core];
        lalrStates[newId[i]].insert(states[i].begin(), states[i].end());
    }

    map<pair<int, char>, int> lalrTrans;
    for (auto& [key, target] : trans) {
        lalrTrans[{newId[key.first], key.second}] = newId[target];
    }
    states = lalrStates;
    trans = lalrTrans;

    cout << "\nNumber of LALR States: " << states.size() << "\n";

    for (int i = 0; i < states.size(); i++) {
        for (Item it : states[i]) {
            string rhs = grammar[it.prod].second;

            if (it.dot < rhs.size()) {
                char sym = rhs[it.dot];
                int nextState = trans[{i, sym}];

                if (terms.count(sym)) {
                    if (!addAction(i, sym, "S" + to_string(nextState))) {
                        cout << "\nGrammar is not LALR(1)\n";
                        return 0;
                    }
                } else {
                    gotoTable[{i, sym}] = nextState;
                }
            } else if (it.prod == 0 && it.lookahead == '$') {
                if (!addAction(i, '$', "ACC")) {
                    cout << "\nGrammar is not LALR(1)\n";
                    return 0;
                }
            } else {
                if (!addAction(i, it.lookahead, "R" + to_string(it.prod))) {
                    cout << "\nGrammar is not LALR(1)\n";
                    return 0;
                }
            }
        }
    }

    set<char> actionCols = terms;
    actionCols.insert('$');
    cout << "\nACTION TABLE\nState\t";
    for (char c : actionCols) cout << c << "\t";
    cout << "\n";
    for (int i = 0; i < states.size(); i++) {
        cout << i << "\t";
        for (char c : actionCols) {
            cout << (actionTable.count({i, c}) ? actionTable[{i, c}] : "-") << "\t";
        }
        cout << "\n";
    }

    cout << "\nGOTO TABLE\nState\t";
    for (char c : nonterms) cout << c << "\t";
    cout << "\n";
    for (int i = 0; i < states.size(); i++) {
        cout << i << "\t";
        for (char c : nonterms) {
            cout << (gotoTable.count({i, c}) ? to_string(gotoTable[{i, c}]) : "-") << "\t";
        }
        cout << "\n";
    }

    string input;
    cout << "\nEnter input string: ";
    cin >> input;
    input += '$';

    vector<int> st = {0};
    int idx = 0;
    cout << "\nPARSER TRACE TABLE\n" << left << setw(25) << "STACK" << setw(20) << "INPUT" << "ACTION\n";

    while (true) {
        string stStr = "";
        for (int s : st) stStr += to_string(s) + " ";

        int curState = st.back();
        char curSym = input[idx];

        cout << left << setw(25) << stStr << setw(20) << input.substr(idx);

        if (!actionTable.count({curState, curSym})) {
            cout << "ERROR\n\nString Rejected\n";
            break;
        }

        string act = actionTable[{curState, curSym}];
        if (act == "ACC") {
            cout << "ACCEPT\n\nString Accepted\n";
            break;
        } else if (act[0] == 'S') {
            int nextState = stoi(act.substr(1));
            cout << "SHIFT " << nextState << "\n";
            st.push_back(nextState);
            idx++;
        } else if (act[0] == 'R') {
            int prodIdx = stoi(act.substr(1));
            char lhs = grammar[prodIdx].first;
            string rhs = grammar[prodIdx].second;

            cout << "REDUCE " << lhs << "->" << (rhs.empty() ? "#" : rhs) << "\n";
            for (int k = 0; k < rhs.size(); k++) st.pop_back();
            st.push_back(gotoTable[{st.back(), lhs}]);
        }
    }

    return 0;
}