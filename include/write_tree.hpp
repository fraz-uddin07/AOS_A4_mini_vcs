
#ifndef WRITE_TREE_HPP
#define WRITE_TREE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class WriteTree {
    public:
        WriteTree();
        ~WriteTree();

        //Function Implementation
        static string tree_writer(const fs::path& directory_path);
        static void write_tree_handler();






};







#endif