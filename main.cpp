
#include <iostream>
#include <vector>
#include <string>
#include "include/init.hpp"
#include "include/hash_object.hpp"
#include "include/cat_file.hpp"
#include "include/write_tree.hpp"
#include "include/ls_tree.hpp"
#include "include/add.hpp"
#include "include/commit.hpp"
#include "include/log.hpp"
#include "include/checkout.hpp"

#define nl '\n'

using namespace std;



int main (int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Command Format : <command> <flags> <arguments>" << nl;
        exit(EXIT_SUCCESS);
    }

    vector<string> commands(argc);
    for (int i = 0; i < argc; i++) {
        commands[i] = string(argv[i]);
    }

    string command = commands[1];
    
    if (command == "init") {
        Init::init_handler();
    }
    else if (command == "hash-object") {
        HashObject::hash_handler(commands);
    }
    else if (command == "cat-file") {
        CatFile::cat_file_handler(commands);
    }
    else if (command == "write-tree") {
        WriteTree::write_tree_handler();
    }
    else if (command == "ls-tree") {
        LsTree::ls_tree_handler(commands);
    }
    else if (command == "add") {
        Add::add_file_handler(commands);
    }
    else if (command == "commit") {
        Commit::commit_handler(commands);
    }
    else if (command == "log") {
        Log::log_handler();
    }
    else if (command == "checkout") {
        Checkout::checkout_handler(commands);
    }
    else if (command == "list-commands") {
        cout << "<init>" << "\n" << "<hash-object> [-w] <file-path" << "\n" << "<cat-file> [-p | -s | -t] <file-sha>" << "\n" << 
        "<write-tree>" << "\n" << "<ls-tree> [--name-only] <tree-sha>" << "\n" << "<add> [.] <file-path(s)>" << "\n" <<
        "<commit> [-m] <commit-message>" << "\n" << "<log>" << "\n" << "<checkout> <commit-sha>" << "\n";
    }
    else {
        cout << "Invalid Command. Type \"list-commands\" for a list of supported commands." << endl;
    }


    return 0;
}
