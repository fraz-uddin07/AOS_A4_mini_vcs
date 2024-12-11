
#ifndef CAT_FILE_HPP
#define CAT_FILE_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CatFile {
    public:
        CatFile();
        ~CatFile();

        //Function Implementation
        static void decompress_file(string& file_path, string& type);
        static void cat_file_handler(vector<string>& commands);






};







#endif