#pragma once
#include <SqliteCpp/SQLiteCpp.h>
#include "Day.h"
#include "Task.h"

class DailiesDatabase
{
private: 
public:
    SQLite::Database OpenAppDatabase(const std::string& path);
    void Migrate(SQLite::Database& db);
    void insertDay(SQLite::Database& db, const Day& day);
    Day findDayByDateString(SQLite::Database& db, std::string dateString);
    void insertTask(SQLite::Database& db, const Day& day, const Task& task);
};

