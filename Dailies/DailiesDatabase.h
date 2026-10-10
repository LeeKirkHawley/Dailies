#pragma once
#include <SqliteCpp/SQLiteCpp.h>


class DailiesDatabase
{
private: 
public:
    SQLite::Database OpenAppDatabase(const std::string& path);
    void Migrate(SQLite::Database& db);
};

