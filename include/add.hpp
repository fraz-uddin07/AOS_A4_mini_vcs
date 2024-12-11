
#ifndef ADD_HPP
#define ADD_HPP

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class Add {
    public:
        Add();
        ~Add();

        //Function Prototypes
        static void add_file_to_index(const fs::path& file_path);
        static void add_file_handler(vector<string>& commands);




};




#endif