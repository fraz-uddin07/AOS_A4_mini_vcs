
#ifndef COMMIT_HPP
#define COMMIT_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Commit {
    public:
        Commit();
        ~Commit();

        //Function Prototypes
        static void commit_handler(vector<string>& commands);




};




#endif