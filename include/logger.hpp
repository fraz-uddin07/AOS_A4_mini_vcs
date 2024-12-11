
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>

using namespace std;

class Logger {
    public:
        Logger();
        ~Logger();

        static string d_ylw;
        static string d_red;
        static string d_grn;
        static string ylw;
        static string red;
        static string grn;
        static string reset;


        //Function Prototypes
        static void print_info_to_console(const string& log_msg);
        static void print_success_to_console(const string& log_msg);
        static void print_failure_to_console(const string& log_msg);
        static void pretty_print_filename(string& result);





};




#endif