#include <iostream>
#include <filesystem>
#include <fstream>
#include "../include/init.hpp"
#include "../include/logger.hpp"

using namespace std;
namespace fs = std::filesystem;


Init::Init() {
    // Constructor
}

Init::~Init() {
    //Destructor
}

//Function Implementation

void Init::init_handler () {
    if (fs::create_directory(".git")) {
        Logger::print_info_to_console("Initialising new git repository...");
        fs::create_directory(".git/objects");
        fs::create_directory(".git/refs");
        ofstream(".git/HEAD");
        Logger::print_success_to_console("Empty git repository initialised");
    }
    else {
        Logger::print_failure_to_console("A .git folder already exists");
    }
    return;
}

