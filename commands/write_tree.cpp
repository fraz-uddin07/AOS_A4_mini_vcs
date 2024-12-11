
#include <iostream>
#include <filesystem>
#include <sstream>
#include "../include/write_tree.hpp"
#include "../include/blob_creator.hpp"

using namespace std;
namespace fs = std::filesystem;

WriteTree::WriteTree () {
    //Constructor
}

WriteTree::~WriteTree () {
    //Destructor
}

string WriteTree::tree_writer(const fs::path& directory_path) {
    ostringstream curr_dir_tree_content;
    
    for (auto& dir_entry : fs::directory_iterator(directory_path)) {
        if (fs::is_regular_file(dir_entry.path())) {
            string curr_blob_sha = BlobCreator::blob_creator(dir_entry.path());
            curr_dir_tree_content << "100644 blob " << curr_blob_sha << " " << dir_entry.path().filename().string() << "\n";
        }
        else if (fs::is_directory(dir_entry.path())) {
            if (dir_entry.path().filename().string() == ".git") { continue; }
            string subtree_sha = tree_writer(dir_entry.path());
            curr_dir_tree_content << "040000 tree " << subtree_sha << " " << dir_entry.path().filename().string() << "\n";
        }
    }
    // string curr_tree_sha_hash = BlobCreator::calculate_sha1_hash(curr_dir_tree_content.str());
    string curr_tree_sha_hash = BlobCreator::create_tree_blob(curr_dir_tree_content.str(), directory_path.filename().string());
    return curr_tree_sha_hash;
}


void WriteTree::write_tree_handler() {
    string src_dir_sha = WriteTree::tree_writer(fs::current_path());
    cout << "Tree SHA : " << src_dir_sha << endl;
    return;
}

