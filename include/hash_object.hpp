
#ifndef HASH_OBJECT_HPP
#define HASH_OBJECT_HPP

#include <vector>
#include <string>

using namespace std;

class HashObject {
    public:
        HashObject();
        ~HashObject();


        //Function Prototypes
        static void hash_handler(vector<string>& commands);
        static string execute_hash(string& file_path, bool write_enabled);









};














#endif