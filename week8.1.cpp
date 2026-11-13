#include <bits/stdc++.h>
using namespace std;

int prec(char c) {
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

void applyOp(stack<string>& val, stack<char>& op) {
    static int tCount = 1;
    string b = val.top(); val.pop();
    string a = val.top(); val.pop();
    char o = op.top(); op.pop();
    string t = "t" + to_string(tCount++);
    cout << t << " = " << a << " " << o << " " << b << "\n";
    val.push(t);
}

string generateTAC(string s) {
    stack<char> op;
    stack<string> val;

    for (int i = 0; i < s.size(); i++) {
        if (isspace(s[i])) continue;

        if (isalnum(s[i])) {
            string operand;
            while (i < s.size() && isalnum(s[i])) operand += s[i++];
            val.push(operand);
            i--;
        } else if (s[i] == '(') {
            op.push('(');
        } else if (s[i] == ')') {
            while (op.top() != '(') applyOp(val, op);
            op.pop();
        } else {
            while (!op.empty() && prec(op.top()) >= prec(s[i])) applyOp(val, op);
            op.push(s[i]);
        }
    }

    while (!op.empty()) applyOp(val, op);
    return val.top();
}

int main() {
    string s, result;
    cout << "Enter expression: ";
    getline(cin, s);
    cout << "\nTHREE ADDRESS CODE\n";
    result = generateTAC(s);
    cout << "Result = " << result << "\n";
    return 0;
}
