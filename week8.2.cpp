#include <bits/stdc++.h>
using namespace std;

struct Node {
    string val;
    Node *left, *right;
    Node(string v, Node* l = nullptr, Node* r = nullptr) 
        : val(v), left(l), right(r) {}
};

int regCount = 0;

int prec(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

void makeSubtree(stack<Node*>& nodes, stack<char>& ops) {
    Node* right = nodes.top(); nodes.pop();
    Node* left = nodes.top(); nodes.pop();
    char op = ops.top(); ops.pop();
    nodes.push(new Node(string(1, op), left, right));
}

Node* buildAST(string exp) {
    stack<Node*> nodes;
    stack<char> ops;

    for (int i = 0; i < exp.length(); i++) {
        if (isspace(exp[i])) continue;

        if (isalnum(exp[i])) {
            string val = "";
            while (i < exp.length() && isalnum(exp[i]))
                val += exp[i++];
            i--;
            nodes.push(new Node(val));
        } else if (exp[i] == '(') {
            ops.push('(');
        } else if (exp[i] == ')') {
            while (ops.top() != '(') makeSubtree(nodes, ops);
            ops.pop();
        } else {
            while (!ops.empty() && prec(ops.top()) >= prec(exp[i]))
                makeSubtree(nodes, ops);
            ops.push(exp[i]);
        }
    }

    while (!ops.empty()) makeSubtree(nodes, ops);
    return nodes.top();
}

void printAST(Node* root, string indent = "") {
    if (!root) return;
    cout << indent << root->val << "\n";
    printAST(root->left, indent + "  |--");
    printAST(root->right, indent + "  |--");
}

int generateCode(Node* root) {
    if (!root->left && !root->right) {
        cout << "MOV R" << regCount << ", " << root->val << "\n";
        return regCount++;
    }

    int leftReg = generateCode(root->left);
    int rightReg = generateCode(root->right);

    if (root->val == "+") cout << "ADD ";
    else if (root->val == "-") cout << "SUB ";
    else if (root->val == "*") cout << "MUL ";
    else if (root->val == "/") cout << "DIV ";

    cout << "R" << leftReg << ", R" << rightReg << "\n";
    return leftReg;
}

int main() {
    string exp;
    cout << "Enter expression: ";
    getline(cin, exp);

    Node* root = buildAST(exp);

    cout << "\n--- ABSTRACT SYNTAX TREE ---\n";
    printAST(root);

    cout << "\n--- MACHINE CODE ---\n";
    generateCode(root);

    return 0;
}