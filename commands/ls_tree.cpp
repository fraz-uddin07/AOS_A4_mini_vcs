
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <zlib.h>
#include "../include/ls_tree.hpp"
#include "../include/logger.hpp"
#include "../include/cat_file.hpp"

using namespace std;
namespace fs = std::filesystem;

LsTree::LsTree() {
    //Constructor
}

LsTree::~LsTree() {
    //Destructor
}

void LsTree::print_tree_content (const string& tree_sha, bool name_only) {
    string file_path = ".git/objects/" + tree_sha.substr(0, 2) + "/" + tree_sha.substr(2);
    if (!fs::exists(file_path)) {
        Logger::print_failure_to_console("No such object exists");
        return;
    }
    ifstream infile_ptr(file_path, ios::binary);
    if (!infile_ptr) {
        Logger::print_failure_to_console("Unable to open compressed file");
    }
    infile_ptr.seekg(0, ios::end);
    streamsize compressed_size = infile_ptr.tellg();
    infile_ptr.seekg(0, ios::beg);

    vector<Bytef> compressed_string(compressed_size);
    if (!infile_ptr.read(reinterpret_cast<char*>(compressed_string.data()), compressed_size)) {
        Logger::print_failure_to_console("Unable to read file");
        return;
    }
    infile_ptr.close();

    //Decompress
    unsigned long decompressed_size = compressed_size * 4;
    vector<Bytef> decompressed_data(decompressed_size);
    int ret_val = uncompress(decompressed_data.data(), &decompressed_size, compressed_string.data(), compressed_size);
    if (ret_val == Z_BUF_ERROR) {
        decompressed_size = compressed_size * 8; // Try with 8x size
        decompressed_data.resize(decompressed_size);
        ret_val = uncompress(decompressed_data.data(), &decompressed_size,
        compressed_string.data(), compressed_size);
    }
    if (ret_val != Z_OK) {
        Logger::print_failure_to_console("Decompression failed");
        return;
    }

    //Print decompressed data
    string result(reinterpret_cast<char*>(decompressed_data.data()), decompressed_size);
    // cout << "idx " << result.find('\0') << endl;
    result = result.substr(result.find('\0') + 1);
    if (name_only) {
        Logger::pretty_print_filename(result);
        return;
    }
    // cout.write(reinterpret_cast<char*>(decompressed_data.data()), decompressed_size);
    cout << result;
    
    return;
}


void LsTree::ls_tree_handler (vector<string>& commands) {
    if (commands.size() != 3 && commands.size() != 4) {
        Logger::print_failure_to_console("Invalid arguments to ls-tree command"); 
        return;
    }
    bool name_only = false;
    string tree_sha;
    if (commands.size() == 4) {
        if (commands[2] == "--name-only") {
            name_only = true;
            tree_sha = commands[3];
        }
        else {
            Logger::print_failure_to_console("Invalid arguments to ls-tree command"); 
            return;
        }
    }
    else if (commands.size() == 3) {
        tree_sha = commands[2];
    }
    LsTree::print_tree_content(tree_sha, name_only);
    
    return;
}
