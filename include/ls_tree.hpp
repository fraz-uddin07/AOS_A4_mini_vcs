
#ifndef LS_TREE_HPP
#define LS_TREE_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class LsTree {
    public:
        LsTree();
        ~LsTree();

        //Function Prototypes
        static void print_tree_content(const string& tree_sha, bool name_only);
        static void ls_tree_handler(vector<string>& commands);





};




#endif