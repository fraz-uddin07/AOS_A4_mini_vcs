
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "../include/add.hpp"
#include "../include/logger.hpp"
#include "../include/blob_creator.hpp"

using namespace std;
namespace fs = std::filesystem;

Add::Add() {
    //Constructor
}

Add::~Add() {
    //Destructor
}

void Add::add_file_to_index(const fs::path& file_path) {
    //Open file in ".git/index", calculate SHA1 hash, push <sha> <file_path> to index file
    ofstream outfile_ptr(".git/index", ios::app);
    if (!outfile_ptr.is_open()) {
        Logger::print_failure_to_console("Unable to open index file");
        return;
    }
    string sha1_hash_file = BlobCreator::blob_creator(file_path);
    if (sha1_hash_file == "") {
        return;
    }
    outfile_ptr << sha1_hash_file << " " << file_path << "\n";
    cout << file_path << " : ";
    Logger::print_success_to_console("Added to staging area");

    return;
}


void Add::add_file_handler(vector<string>& commands) {
    if (commands.size() < 3) {
        Logger::print_failure_to_console("Invalid arguments to \"add\" command");
        return;
    }
    
    string src_path = "/";
    if (commands[2] == ".") {
        src_path = ".";
    }

    if (src_path == ".") {
        for (const auto& dir_entry : fs::recursive_directory_iterator(".")) {
            if (dir_entry.path().string().find(".git") != string::npos) {
                continue;
            }
            if (fs::is_regular_file(dir_entry.path())) {
                add_file_to_index(dir_entry.path());
            }
        }
    }
    else {
        for (int i = 2; i < commands.size(); i++) {
            const fs::path file_path = commands[i];
            add_file_to_index(file_path);
        }
    }

    return;
}