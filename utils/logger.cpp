
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../include/logger.hpp"

using namespace std;

Logger::Logger() {
    //Constructor
}

Logger::~Logger() {
    //Destructor
}

string Logger::d_ylw = "\033[33m";
string Logger::d_red = "\033[31m";
string Logger::d_grn = "\033[32m";
string Logger::ylw = "\033[93m";
string Logger::red = "\033[91m";
string Logger::grn = "\033[92m";
string Logger::reset = "\033[0m";

void Logger::print_info_to_console (const string& info_msg) {
    cout << d_ylw << info_msg << reset << endl;
    return;
}

void Logger::print_success_to_console (const string& info_msg) {
    cout << d_grn << "[ OK ] : " << info_msg << reset << endl;
    return;
}

void Logger::print_failure_to_console (const string& info_msg) {
    cout << d_red << "[ ERROR ] : " << info_msg << reset << endl;
    return;
}

void Logger::pretty_print_filename(string& result) {
    vector<string> tokens;
    istringstream tok_stream(result);
    string token;

    while(getline(tok_stream, token, '\n')) {
        istringstream line_stream(token);
        while(getline(line_stream, token, ' ')) {
            tokens.push_back(token);
        }
    }
    for (int i = 3; i < tokens.size(); i = i + 4) {
        cout << tokens[i] << endl;
    }

    return;
}