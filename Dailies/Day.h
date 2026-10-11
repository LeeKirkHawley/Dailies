#pragma once
#include <string>

struct Day {
    int64_t     id = 0;          // INTEGER PRIMARY KEY
    std::string date;
};

//struct Person {
//    int64_t     id = 0;          // INTEGER PRIMARY KEY
//    std::string name;
//    int         age = 0;
//    double      height = 0.0;
//    std::string email;           // can be empty / NULL
//};