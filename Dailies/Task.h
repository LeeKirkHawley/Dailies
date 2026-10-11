#pragma once
#include <string>

struct Task {
    int64_t     id = 0;          // INTEGER PRIMARY KEY
    int64_t     dayId;
    std::string taskName;
    std::string isDone;
    std::string createdUtc;
    std::string type;           // One-Time or Recurring
};

//struct Person {
//    int64_t     id = 0;          // INTEGER PRIMARY KEY
//    std::string name;
//    int         age = 0;
//    double      height = 0.0;
//    std::string email;           // can be empty / NULL
//};#pragma once
