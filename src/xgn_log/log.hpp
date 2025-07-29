#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

namespace xgn {

int filter = 0;

map<string, string> log_codes;

// DEBUG - For debugging
// INFO - Log normal activities
// WARNING - Unusual activities
// ERROR - Something went wrong but can still run
// CRITICAL - Something went wrong and can only run limited-ly
// FATAL - Something went wrong and must quit

// Read log codes from log_codes.txt
void read_log_codes(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Code 0x9000: Cannot open log code list file: " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            if (line.substr(0, pos) == "#") {} else {
                string code = line.substr(0, pos);
                string message = line.substr(pos + 1);
                log_codes[code] = message;
            }
        }
    }
    file.close();
}

void log(const string& code, const int& level, const string& extra_info = "") {
    read_log_codes("./src/xgn_log/log_codes.txt"); // Load log codes from file
    if (level == 0 || filter <= 0) {
        cout << "[DEBUG] ";
    } else if (level == 1 || filter <= 1) {
        cout << "[INFO] ";
    } else if (level == 2 || filter <= 2) {
        cout << "[WARNING] ";
    } else if (level == 3 || filter <= 3) {
        cout << "[ERROR] ";
    } else if (level == 4 || filter <= 4) {
        cout << "[CRITICAL] ";
    } else if (level == 5 || filter <= 5) {    
        cout << "[FATAL] ";
    } else {
        cout << "[UNKNOWN] ";
    }
    cout << "Code " << code << ": ";
    cout << log_codes[code] << " ";
    cout << extra_info << endl;
}

};

#endif // LOG_HPP