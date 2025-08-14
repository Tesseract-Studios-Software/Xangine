// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

namespace xgn {

int log_filter = 1;

map<string, string> log_codes;
string log_code_path;

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

int log(const string& code, const int& level, const string& extra_info = "") {
    if (log_codes.empty()) {read_log_codes(log_code_path);} // Load log codes from file
    if (level == 0) {
        if (log_filter > 0) {return 1;}
        cout << "[DEBUG] ";
    } else if (level == 1) {
        if (log_filter > 1) {return 1;}
        cout << "[INFO] ";
    } else if (level == 2) {
        if (log_filter > 2) {return 1;}
        cout << "[WARNING] ";
    } else if (level == 3) {
        if (log_filter > 3) {return 1;}
        cout << "[ERROR] ";
    } else if (level == 4) {
        if (log_filter > 4) {return 1;}
        cout << "[CRITICAL] ";
    } else if (level == 5) {
        if (log_filter > 5) {return 1;}
        cout << "[FATAL] ";
    } else {
        cout << "[UNKNOWN] ";
    }
    cout << "Code " << code << ": ";
    if (log_codes.count(code)) {
        cout << log_codes[code] << " ";
    } else {
        cout << "Unknown code ";
    }
    cout << extra_info << endl;
    return 0;
}

};

#endif // LOG_HPP