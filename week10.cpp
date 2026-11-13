#include <bits/stdc++.h>
using namespace std;

string toBinary(string s)
{
    string bin = "";

    for (char c : s) {
        for (int i = 7; i >= 0; i--) {
            bin += ((c >> i) & 1) ? '1' : '0';
        }
    }

    return bin;
}

string calculateCRC(string data, string poly)
{
    int n = poly.length();

    string temp = data + string(n - 1, '0');

    for (int i = 0; i <= temp.length() - n; i++) {
        if (temp[i] == '1')
        {
            for (int j = 0; j < n; j++) {
                temp[i + j] = (temp[i + j] == poly[j]) ? '0' : '1';
            }
        }
    }

    return temp.substr(temp.length() - n + 1);
}

bool verifyCRC(string received, string poly)
{
    int n = poly.length();

    if (received.length() < n)
        return false;

    string temp = received;

    for (int i = 0; i <= temp.length() - n; i++) {
        if (temp[i] == '1')
        {
            for (int j = 0; j < n; j++) {
                temp[i + j] = (temp[i + j] == poly[j]) ? '0' : '1';
            }
        }
    }

    string remainder = temp.substr(temp.length() - n + 1);

    cout << "Remainder: " << remainder << endl;

    return remainder.find('1') == string::npos;
}

int main()
{
    string str;

    cout << "Enter a string: ";
    cin >> str;

    string data = toBinary(str);

    cout << "\nBinary Data: " << data << endl;

    string crc12 = "1100000001111";
    string crc16 = "11000000000000101";
    string ccitt = "10001000000100001";

    string r12 = calculateCRC(data, crc12);
    string r16 = calculateCRC(data, crc16);
    string rccitt = calculateCRC(data, ccitt);

    cout << "\nTransmitted Data:" << endl;

    cout << "CRC-12    : " << data + r12 << endl;
    cout << "CRC-16    : " << data + r16 << endl;
    cout << "CRC-CCITT : " << data + rccitt << endl;

    string received;

    cout << "\nEnter received code for CRC-12: ";
    cin >> received;

    cout << "\nCRC-12 Verification: ";
    cout << (verifyCRC(received, crc12) ? "No Error Detected" : "Error Detected") << endl;

    cout << "\nEnter received code for CRC-16: ";
    cin >> received;

    cout << "\nCRC-16 Verification: ";
    cout << (verifyCRC(received, crc16) ? "No Error Detected" : "Error Detected") << endl;

    cout << "\nEnter received code for CRC-CCITT: ";
    cin >> received;

    cout << "\nCRC-CCITT Verification: ";
    cout << (verifyCRC(received, ccitt) ? "No Error Detected" : "Error Detected") << endl;

    return 0;
}
