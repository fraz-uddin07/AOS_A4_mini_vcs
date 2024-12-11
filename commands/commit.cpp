
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../include/commit.hpp"
#include "../include/logger.hpp"
#include "../include/blob_creator.hpp"


using namespace std;
namespace fs = std::filesystem;

Commit::Commit() {
    //Constructor
}

Commit::~Commit() {
    //Destructor
}


void Commit::commit_handler(vector<string>& commands) {
    if (commands.size() != 2 && commands.size() != 4) {
        Logger::print_failure_to_console("Invalid arguments to \"commmit\" command");
        return;
    }
    string message = "";
    if (commands.size() == 2) {
        message = "Default commit message";
    }
    else {
        message = commands[3];
    }

    //Read the index file
    ifstream idx_file(".git/index");
    if (!idx_file.is_open()) {
        Logger::print_failure_to_console("Unable to open index file");
        return;
    }

    ostringstream tree_data;
    string line_data;
    while (getline(idx_file, line_data)) {
        tree_data << line_data << "\n";
    }
    idx_file.close();

    Logger::print_info_to_console("Hashing Index file");
    string tree_sha1_hash = BlobCreator::calculate_sha1_hash(tree_data.str());
    ofstream tree_file(".git/objects/" + tree_sha1_hash, ios::binary);
    tree_file << tree_data.str();
    tree_file.close();

    string user_data = "fraz-uddin07";
    string commit_data = "tree " + tree_sha1_hash + "\n";
    commit_data = commit_data + "commit_message " + message + "\n";
    commit_data = commit_data + "timestamp " + to_string(time(nullptr)) + "\n";
    commit_data = commit_data + "committed_by " + user_data + "\n\n";
    Logger::print_info_to_console("Commit object created");

    string commit_data_sha1 = BlobCreator::calculate_sha1_hash(commit_data);
    ofstream commit_file(".git/objects/" + commit_data_sha1, ios::binary);
    commit_file << commit_data;
    commit_file.close();

    //Update HEAD
    ofstream head_file(".git/HEAD");
    head_file << commit_data_sha1;
    head_file.close();

    //Update Log file
    fstream log_file(".git/Log", ios::in | ios::out | ios::app);
    // log_file << commit_data_sha1 << "\n";
    if (!log_file.is_open()) {
        Logger::print_failure_to_console("Unable to write to Log file");
    }
    else {
        //Write to Log file
        log_file.seekg(0, ios::beg);
        string original_data;
        original_data.assign((istreambuf_iterator<char>(log_file)), istreambuf_iterator<char>());
        log_file.close();

        ofstream log_file_out(".git/Log", ios::out | ios::trunc);
        if (!log_file_out.is_open()) {
            Logger::print_failure_to_console("Unable to update Log file");
        }
        else {
            log_file_out << commit_data_sha1 << "\n" << original_data;
            log_file.close();
            Logger::print_info_to_console("Log file updated");
        }
        // log_file.seekp(0, ios::beg);
        // log_file << commit_data_sha1 << "\n" << original_data;
    }

    Logger::print_success_to_console("Commit created successfully");
    cout << "Commit SHA1 : " << commit_data_sha1 << endl;

    return;
}