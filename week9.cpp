#include <bits/stdc++.h>
using namespace std;

// Bit Stuffing
string bit_stuff_sender(string data) {
    string stuffed = "";
    int count = 0;

    for (char bit : data) {
        stuffed += bit;

        if (bit == '1') {
            count++;
            if (count == 5) {
                stuffed += '0';
                count = 0;
            }
        } else count = 0;
    }

    return stuffed;
}

// Bit De-stuffing
string bit_stuff_receiver(string data) {
    string original = "";
    int count = 0;

    for (int i = 0; i < data.length(); i++) {
        original += data[i];

        if (data[i] == '1') {
            count++;
            if (count == 5) {
                i++;
                count = 0;
            }
        } else count = 0;
    }
    return original;
}

string bit_stuff_flag(string data) {
    string flag = "01111110";
    return flag + bit_stuff_sender(data) + flag;
}

string bit_unstuff_frame(string frame, bool& valid) {
    string flag = "01111110";

    if (frame.length() >= 16 && frame.substr(0, 8) == flag && frame.substr(frame.length() - 8) == flag) {
        valid = true;
        return bit_stuff_receiver(frame.substr(8, frame.length() - 16));
    }

    cout << "Error: Invalid frame! Missing start or end FLAG." << endl;
    valid = false;
    return "";
}

void run_bit_stuffing() {

    cout << "BIT STUFFING PROGRAM" << endl;
    cout << "FLAG Pattern = 01111110" << endl;

    cout << "\nEnter binary data bits (0s and 1s): ";
    string data;
    cin >> data;

    for (char b : data) {
        if (b != '0' && b != '1') {
            cout << "Error: Input must contain only '0' and '1'." << endl;
            return;
        }
    }

    bool valid;
    string stuffed = bit_stuff_sender(data);
    string frame = bit_stuff_flag(data);
    string received = bit_unstuff_frame(frame, valid);

    cout << "BIT STUFFING RESULT" << endl;
    cout << "Original Data  : " << data << endl;
    cout << "Stuffed Data   : " << stuffed << endl;
    cout << "Framed Data    : " << frame << endl;
    cout << "Recovered Data : " << received << endl;

    if (valid && data == received)
        cout << "\nSUCCESS" << endl;
    else
        cout << "\nERROR" << endl;
}

void print_hex_vector(const vector<int>& a) {
    cout << "[";
    for (int i = 0; i < a.size(); i++) {
        cout << "0x" << hex << setw(2) << setfill('0') << a[i] << dec;
        if (i + 1 < a.size()) cout << ", ";
    }
    cout << "]" << endl;
}

void print_dec_vector(const vector<int>& a) {
    cout << "[";
    for (int i = 0; i < a.size(); i++) {
        cout << a[i];
        if (i + 1 < a.size()) cout << ", ";
    }
    cout << "]" << endl;
}

// Character Stuffing
vector<int> char_stuff_sender(const vector<int>& data) {
    int FLAG = 0x7E, ESC = 0x7D;
    vector<int> stuffed;

    cout << "\n========== SENDER SIDE ==========" << endl;
    cout << "Original Data: ";
    print_hex_vector(data);

    for (int b : data) {
        if (b == FLAG || b == ESC) {
            stuffed.push_back(ESC);
            stuffed.push_back(b ^ 0x20);
            cout << "Special byte found: 0x" << hex << setw(2)
                 << setfill('0') << b << " -> ESC (0x" << setw(2)
                 << ESC << ") + 0x" << setw(2) << (b ^ 0x20)
                 << dec << endl;
        } else {
            stuffed.push_back(b);
            cout << "Normal byte: 0x" << hex << setw(2)
                 << setfill('0') << b << dec
                 << " -> No stuffing required" << endl;
        }
    }

    cout << "\nStuffed Data: ";
    print_hex_vector(stuffed);
    return stuffed;
}

// Character De-stuffing
vector<int> char_stuff_receiver(const vector<int>& data) {
    vector<int> original;

    cout << "\n========== RECEIVER SIDE ==========" << endl;
    cout << "Received Stuffed Data: ";
    print_hex_vector(data);

    for (int i = 0; i < data.size(); i++) {
        if (data[i] == 0x7D) {
            cout << "\nESC found at position " << dec << i
                 << ": 0x" << hex << setw(2) << setfill('0')
                 << data[i] << dec << endl;

            i++;
            if (i < data.size()) {
                int b = data[i] ^ 0x20;
                original.push_back(b);

                cout << "Escaped byte: 0x" << hex << setw(2)
                     << setfill('0') << data[i]
                     << " -> Recovered original byte: 0x"
                     << setw(2) << b << dec << endl;
            }
        } else {
            original.push_back(data[i]);
            cout << "Normal byte: 0x" << hex << setw(2)
                 << setfill('0') << data[i] << dec
                 << " -> No de-stuffing required" << endl;
        }
    }

    cout << "\nRecovered Original Data: ";
    print_hex_vector(original);
    return original;
}

void run_character_stuffing() {
    cout << "\n============================================" << endl;
    cout << "         CHARACTER STUFFING PROGRAM" << endl;
    cout << "============================================" << endl;
    cout << "\nSpecial Characters:" << endl;
    cout << "FLAG = 0x7E (01111110)" << endl;
    cout << "ESC  = 0x7D (01111101)" << endl;
    cout << "\nEnter the data bytes separated by spaces." << endl;
    cout << "Example: 10 20 7E 30 7D 40" << endl;

    cout << "\nEnter data bytes in hexadecimal: ";
    string input;
    getline(cin >> ws, input);

    vector<int> data;
    stringstream ss(input);
    string token;

    while (ss >> token) {
        try {
            int b = stoi(token, nullptr, 16);
            if (b < 0 || b > 255) throw 1;
            data.push_back(b);
        } catch (...) {
            cout << "Error: Invalid hexadecimal input '" << token << "'." << endl;
            return;
        }
    }

    cout << "\nOriginal Input Data (Decimal): ";
    print_dec_vector(data);

    vector<int> stuffed = char_stuff_sender(data);

    cout << "Data transmitted through the channel (Decimal):" << endl;
    print_dec_vector(stuffed);

    vector<int> received = char_stuff_receiver(stuffed);

    cout << "CHARACTER STUFFING FINAL RESULT" << endl;

    cout << "Original Data : ";
    print_dec_vector(data);
    cout << "Stuffed Data  : ";
    print_dec_vector(stuffed);
    cout << "Received Data : ";
    print_dec_vector(received);

    if (data == received)
        cout << "\nSUCCESS: Original and received data are identical." << endl;
    else
        cout << "\nERROR: Data mismatch occurred." << endl;
}

int main() {
    while (true) {
        cout << "1. Bit Stuffing" << endl;
        cout << "2. Character Stuffing" << endl;
        cout << "3. Exit" << endl;

        cout << "\nSelect an option (1-3): ";
        string choice;
        cin >> choice;

        if (choice == "1")
            run_bit_stuffing();
        else if (choice == "2")
            run_character_stuffing();
        else if (choice == "3") {
            cout << "Exiting program." << endl;
            break;
        }
        else
            cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
    }

    return 0;
}
