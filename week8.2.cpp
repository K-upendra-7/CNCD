#include <bits/stdc++.h>
using namespace std;

struct Node {
    string val;
    Node *left, *right;
    Node(string v) : val(v), left(NULL), right(NULL) {}
};

int reg = 0;

int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

void applyOp(stack<Node*>& values, stack<char>& ops) {
    Node *root = new Node(string(1, ops.top()));
    ops.pop();
    root->right = values.top(); values.pop();
    root->left = values.top(); values.pop();
    values.push(root);
}

Node* buildTree(string s) {
    stack<Node*> values;
    stack<char> ops;

    for (int i = 0; i < s.size(); i++) {
        if (isspace(s[i])) continue;

        if (isalnum(s[i])) {
            string operand;
            while (i < s.size() && isalnum(s[i]))
                operand += s[i++];
            i--;
            values.push(new Node(operand));
        } else if (s[i] == '(') {
            ops.push(s[i]);
        } else if (s[i] == ')') {
            while (!ops.empty() && ops.top() != '(')
                applyOp(values, ops);
            if (!ops.empty()) ops.pop();
        } else {
            while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(s[i]))
                applyOp(values, ops);
            ops.push(s[i]);
        }
    }

    while (!ops.empty())
        applyOp(values, ops);

    return values.empty() ? NULL : values.top();
}

vector<string> canvas(15, string(50, ' '));

void draw(Node* root, int r, int c) {
    if (!root) return;
    canvas[r].replace(c, root->val.size(), root->val);
    if (root->left) {
        canvas[r + 1][c - 1] = '/';
        draw(root->left, r + 2, c - 2);
    }
    if (root->right) {
        canvas[r + 1][c + 1] = '\\';
        draw(root->right, r + 2, c + 2);
    }
}

void printAST(Node* root) {
    draw(root, 0, 10);
    for (string& line : canvas)
        if (line.find_first_not_of(' ') != string::npos)
            cout << line.substr(6, line.find_last_not_of(' ') - 5) << "\n";
}

int generateCode(Node *root) {
    if (!root->left && !root->right) {
        cout << "MOV R" << reg << ", " << root->val << "\n";
        return reg++;
    }

    int left = generateCode(root->left);
    int right = generateCode(root->right);

    string op = (root->val == "+") ? "ADD" :
                (root->val == "-") ? "SUB" :
                (root->val == "*") ? "MUL" : "DIV";

    cout << op << " R" << left << ", R" << right << "\n";
    return left;
}

int main() {
    string expression;
    cout << "Enter expression: ";
    getline(cin, expression);

    Node *root = buildTree(expression);
    if (!root) return 0;

    cout << "\nABSTRACT SYNTAX TREE\n";
    printAST(root);

    cout << "\nMACHINE CODE\n";
    generateCode(root);

    return 0;
}