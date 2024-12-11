
#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Log {
    public:
        Log();
        ~Log();

        //Function Prototypes
        static bool starts_with_prefix (string& str, const string prefix);
        static void log_handler();




};




#endif