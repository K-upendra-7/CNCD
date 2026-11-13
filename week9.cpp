#include <bits/stdc++.h>
using namespace std;

string bitStuff(string data) {
    string res = "";
    int ones = 0;
    for (char c : data) {
        res += c;
        if (c == '1') {
            ones++;
            if (ones == 5) {
                res += '0';
                ones = 0;
            }
        } else {
            ones = 0;
        }
    }
    return res;
}

string bitUnstuff(string data) {
    string res = "";
    int ones = 0;
    for (int i = 0; i < data.length(); i++) {
        res += data[i];
        if (data[i] == '1') {
            ones++;
            if (ones == 5) {
                i++;
                ones = 0;
            }
        } else {
            ones = 0;
        }
    }
    return res;
}

void runBitStuffing() {
    string flag = "01111110", data;
    cout << "\nEnter binary data (e.g., 011111101): ";
    cin >> data;

    string stuffed = bitStuff(data);
    string frame   = flag + stuffed + flag;
    string unstuffed = bitUnstuff(stuffed);

    cout << "Stuffed Data   : " << stuffed << endl;
    cout << "Framed Data    : " << frame << endl;
    cout << "Recovered Data : " << unstuffed << endl;
}

void printHex(const vector<int>& v) {
    for (int b : v) cout << "0x" << hex << uppercase << b << " ";
    cout << dec << endl;
}

vector<int> charStuff(const vector<int>& data) {
    int FLAG = 0x7E, ESC = 0x7D;
    vector<int> res;
    for (int b : data) {
        if (b == FLAG || b == ESC) {
            res.push_back(ESC);
            res.push_back(b ^ 0x20);
        } else {
            res.push_back(b);
        }
    }
    return res;
}

vector<int> charUnstuff(const vector<int>& data) {
    int ESC = 0x7D;
    vector<int> res;
    for (int i = 0; i < data.size(); i++) {
        if (data[i] == ESC) {
            i++;
            res.push_back(data[i] ^ 0x20);
        } else {
            res.push_back(data[i]);
        }
    }
    return res;
}

void runCharStuffing() {
    int n;
    cout << "\nEnter number of bytes: ";
    cin >> n;

    vector<int> data(n);
    cout << "Enter " << n << " hex bytes (e.g., 10 7E 7D 20): ";
    for (int i = 0; i < n; i++) {
        cin >> hex >> data[i];
    }
    cout << dec;

    vector<int> stuffed   = charStuff(data);
    vector<int> unstuffed = charUnstuff(stuffed);

    cout << "Stuffed Data   : "; printHex(stuffed);
    cout << "Recovered Data : "; printHex(unstuffed);
}

int main() {
    int choice;
    while (true) {
        cout << "\n1. Bit Stuffing\n 2. Character Stuffing\n3. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) runBitStuffing();
        else if (choice == 2) runCharStuffing();
        else if (choice == 3) break;
        else cout << "Invalid choice!\n";
    }
    return 0;
}