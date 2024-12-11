
#ifndef CHECKOUT_HPP
#define CHECKOUT_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Checkout {
    public:
        Checkout();
        ~Checkout();

        //Function Prototypes
        static void checkout_handler(vector<string>& commands);



};




#endif