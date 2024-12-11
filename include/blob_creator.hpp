
#ifndef BLOB_CREATOR_HPP
#define BLOB_CREATOR_HPP

#include <vector>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class BlobCreator {
    public:
        BlobCreator();
        ~BlobCreator();


        //Function Prototypes
        static string blob_creator(const fs::path& file_path);
        static string calculate_sha1_hash(const string& input_str);
        static string calculate_sha1_hash_file(const string& file_path);
        static string create_tree_blob(string tree_data, const string& dir_name);
        static void restore_file(string& file_sha, string& file_name);








};














#endif