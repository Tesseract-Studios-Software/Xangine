#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

namespace xgn {

map<int, string> log_codes;

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
        cout << "[ERROR] Code 100: Could not open log code list file: " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            int code = stoi(line.substr(0, pos));
            string message = line.substr(pos + 1);
            log_codes[code] = message;
        }
    }
    file.close();
}

void log(const string& code, const int& level, const string& extra_info = "") {
    read_log_codes("./src/xgn_log/log_codes.txt"); // Load log codes from file
    if (level == 0) {
        cout << "[DEBUG] ";
    } else if (level == 1) {
        cout << "[INFO] ";
    } else if (level == 2) {
        cout << "[WARNING] ";
    } else if (level == 3) {
        cout << "[ERROR] ";
    } else if (level == 4) {
        cout << "[CRITICAL] ";
    } else if (level == 5) {    
        cout << "[FATAL] ";
    } else {
        cout << "[UNKNOWN] ";
    }
    cout << "Code " << code << ": ";
    // Get the log message from log_codes
    int code_int = stoi(code);
    if (log_codes.find(code_int) != log_codes.end()) {
        cout << log_codes[code_int] << " ";
    } else {
        cout << "Unknown log code. ";
    }
    cout << extra_info << endl;
}

};

#endif // LOG_HPP