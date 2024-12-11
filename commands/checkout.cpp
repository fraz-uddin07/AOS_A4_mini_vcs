
#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>
#include "../include/checkout.hpp"
#include "../include/blob_creator.hpp"
#include "../include/logger.hpp"

using namespace std;
namespace fs = std::filesystem;

Checkout::Checkout () {
    //Constructor
}

Checkout::~Checkout () {
    //Destructor
}

void Checkout::checkout_handler(vector<string>& commands) {
    if (commands.size() != 3) {
        Logger::print_failure_to_console("Invalid arguments to \"checkout\" command");
        return;
    }
    string commit_sha = commands[2];

    ifstream commit_file_ptr(".git/objects/" + commit_sha);
    if (!commit_file_ptr.is_open()) {
        cout << commit_sha << " : ";
        Logger::print_failure_to_console("No such commit found");
        return;
    }

    string line_data, tree_sha;
    getline(commit_file_ptr, line_data);
    tree_sha = line_data.substr(5);

    // Retrieve tree and file blobs from the object storage
    ifstream tree_file(".git/objects/" + tree_sha);
    while (getline(tree_file, line_data)) {
        istringstream file_hash_path_stream(line_data);
        string file_sha, file_name;
        file_hash_path_stream >> file_sha >> file_name;
        // fs::path file_path = file_name;

        // Restore each file - Decompress each file BLOB
        file_name = file_name.substr(1, file_name.size() - 2);
        cout << "Restoring file " << "\"" << file_name << "\"" << endl;
        BlobCreator::restore_file(file_sha, file_name);

        cout << file_name << " : ";
        Logger::print_success_to_console("File Restored");
    }

    return;
}