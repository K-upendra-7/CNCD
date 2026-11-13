#include <bits/stdc++.h>
using namespace std;

int tempCount = 1;

string newTemp(){
    return "t" + to_string(tempCount++);
}

int precedence(char op) {
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

string generateTAC(string s) {
    stack<char> op;
    stack<string> val;

    for (int i = 0; i < s.size(); i++) {
        if (isspace(s[i]))
            continue;

        if (isalnum(s[i])){
            string operand;

            while (i < s.size() && isalnum(s[i]))
                operand += s[i++];

            i--;
            val.push(operand);
        }
        else if (s[i] == '(')
            op.push(s[i]);

        else if (s[i] == ')')
        {
            while (!op.empty() && op.top() != '(')
            {
                string b = val.top();
                val.pop();

                string a = val.top();
                val.pop();

                char oper = op.top();
                op.pop();

                string t = newTemp();

                cout << t << " = " << a << " "
                     << oper << " " << b << "\n";

                val.push(t);
            }

            if (!op.empty())
                op.pop();
        }
        else
        {
            while (!op.empty() && precedence(op.top()) >= precedence(s[i]))
            {
                string b = val.top();
                val.pop();

                string a = val.top();
                val.pop();

                char oper = op.top();
                op.pop();

                string t = newTemp();

                cout << t << " = " << a << " "
                     << oper << " " << b << "\n";

                val.push(t);
            }

            op.push(s[i]);
        }
    }

    while (!op.empty())
    {
        string b = val.top();
        val.pop();

        string a = val.top();
        val.pop();

        char oper = op.top();
        op.pop();

        string t = newTemp();

        cout << t << " = " << a << " " << oper << " " << b << "\n";

        val.push(t);
    }

    return val.top();
}

int main()
{
    string s, result, variable;

    cout << "Enter expression: ";
    getline(cin, s);

    cout << "\nTHREE ADDRESS CODE\n";

    result = generateTAC(s);

    cout << "Result = " << result << "\n";

    return 0;
}
