#include <bits/stdc++.h>
using namespace std;

string getRemainder(string data, string poly) {
    int n = poly.length();
    for (int i = 0; i <= (int)data.length() - n; i++) {
        if (data[i] == '1') {
            for (int j = 0; j < n; j++) {
                data[i + j] = (data[i + j] == poly[j]) ? '0' : '1';
            }
        }
    }
    return data.substr(data.length() - n + 1);
}

void runCRC(string name, string data, string poly) {
    string zeros(poly.length() - 1, '0');
    string rem = getRemainder(data + zeros, poly);
    
    cout << "\n--- " << name << " ---\n";
    cout << "Remainder   : " << rem << "\n";
    cout << "Transmitted : " << data + rem << "\n";

    string received;
    cout << "Enter received code: ";
    cin >> received;

    string checkRem = getRemainder(received, poly);
    cout << "Check Rem   : " << checkRem << "\n";

    if (checkRem.find('1') == string::npos)
        cout << "Status      : No Error Detected\n";
    else
        cout << "Status      : Error Detected\n";
}

int main() {
    
    string str, data = "";
    cout << "\n=== STANDARD CRCs ===\n";
    cout << "Enter a text string: ";
    cin >> str;
    
    for (char c : str) {
        data += bitset<8>(c).to_string();
    }
    
    cout << "Binary Data : " << data << "\n";
    
    runCRC("Basic CRC", data, "1101");
    runCRC("CRC-12",    data, "1100000001111");
    runCRC("CRC-16",    data, "11000000000000101");
    runCRC("CRC-CCITT", data, "10001000000100001");

    return 0;
}