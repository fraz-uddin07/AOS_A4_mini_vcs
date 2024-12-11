
#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include "../include/log.hpp"
#include "../include/logger.hpp"

using namespace std;
namespace fs = std::filesystem;

Log::Log () {
    //Constructor
}

Log::~Log () {
    //Destructor
}

bool Log::starts_with_prefix (string& str, const string prefix) {
    return str.substr(0, prefix.size()) == prefix;
}

void Log::log_handler () {
    cout << endl;
    ifstream LOG_reader(".git/Log");
    if (!LOG_reader.is_open()) {
        Logger::print_info_to_console("No commit history found");
        return;
    }
    vector<string> parent_shas;
    string parent_sha;
    while (getline(LOG_reader, parent_sha)) {
        parent_shas.push_back(parent_sha);
    }
    parent_shas.push_back("N/A");
    LOG_reader.clear();
    LOG_reader.seekg(0, ios::beg);
    string latest_commit_sha1;
    // getline(LOG_reader, latest_commit_sha1);

    int i = 1;
    while (getline(LOG_reader, latest_commit_sha1)) {
        ifstream commit_file_ptr(".git/objects/" + latest_commit_sha1);
        if (!commit_file_ptr.is_open()) {
            cout << latest_commit_sha1 << " : ";
            Logger::print_failure_to_console("Could not open commit object file");
            continue;
        }
        string line_data = "";
        string tree_sha, commit_message, timestamp_data, committed_by;
        while (getline(commit_file_ptr, line_data)){
            if (starts_with_prefix(line_data, "tree")) {
                tree_sha = line_data.substr(5);
            }
            else if (starts_with_prefix(line_data, "commit_message")) {
                commit_message = line_data.substr(15);
            }
            else if (starts_with_prefix(line_data, "timestamp")) {
                timestamp_data = line_data.substr(10);
            }
            else if (starts_with_prefix(line_data, "committed_by")) {
                committed_by = line_data.substr(13);
            }
        }
        commit_file_ptr.close();

        time_t commit_time = stoi(timestamp_data);
        string formatted_time = asctime(localtime(&commit_time));

        cout << "Commit SHA : " << "\033[33m" << latest_commit_sha1  << "\033[0m" << endl;
        cout << "Tree SHA : " << tree_sha << endl;
        cout << "Parent SHA : " << parent_shas[i++] << endl;
        cout << "Commit Message : " << commit_message << endl;
        cout << "Timestamp : " << timestamp_data << endl;
        cout << "Committed By : " << committed_by << endl;
        cout << "--------------------------------------------------------" << endl << endl;

    }

    LOG_reader.close();

    return;
}