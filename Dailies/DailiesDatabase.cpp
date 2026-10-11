#include "pch.h"
#include "DailiesDatabase.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <stdexcept>
#include <string>
#include "Day.h"
#include "Task.h"

void DailiesDatabase::Migrate(SQLite::Database& db)
{
    try
    {
        SQLite::Transaction tx(db);
        int version = db.execAndGet("PRAGMA user_version").getInt();

        if (version < 1)
        {
            db.exec(R"(
                CREATE TABLE IF NOT EXISTS Days (
                    Id INTEGER PRIMARY KEY,
                    Date TEXT NOT NULL UNIQUE
                );

                CREATE TABLE IF NOT EXISTS Tasks (
                    Id INTEGER PRIMARY KEY,
                    DayId INTEGER NOT NULL REFERENCES Days(Id),
                    TaskName TEXT NOT NULL,
                    IsDone TEXT NULL,
                    CreatedUtc TEXT NOT NULL,
                    Type TEXT NOT NULL
                );
            )");
            version = 1;
        }

        //if (version < 2)
        //{
        //    db.exec("ALTER TABLE Project ADD COLUMN Notes TEXT NOT NULL DEFAULT '';");
        //    version = 2;
        //}

        //if (version < 3)
        //{
        //    db.exec(R"(
        //        CREATE TABLE IF NOT EXISTS Task (
        //            Id INTEGER PRIMARY KEY,
        //            ProjectId INTEGER NOT NULL REFERENCES Project(Id),
        //            Title TEXT NOT NULL
        //        );
        //        CREATE INDEX IF NOT EXISTS IX_Task_ProjectId ON Task(ProjectId);
        //    )");
        //    version = 3;
        //}

        db.exec("PRAGMA user_version = " + std::to_string(version));
        tx.commit();
    }
    catch (const std::exception& ex)
    {
        throw winrt::hresult_error(E_FAIL, winrt::to_hstring(ex.what()));
    }
}

SQLite::Database DailiesDatabase::OpenAppDatabase(const std::string& path)
{
    try
    {
        // SQLITE_OPEN_READWRITE = 0x00000002, SQLITE_OPEN_CREATE = 0x00000004
        SQLite::Database db(path, 0x00000002 | 0x00000004);
        db.exec("PRAGMA foreign_keys = ON;");
        //Migrate(db);
        return db;
    }
    catch (const std::exception& ex)
    {
        throw winrt::hresult_error(E_FAIL, winrt::to_hstring(ex.what()));
    }
}

void DailiesDatabase::insertDay(SQLite::Database& db, const Day& day)
{
    SQLite::Statement stmt(db,
        "INSERT INTO Days (Date) VALUES (?)");

    SQLite::Transaction transaction(db);
    stmt.bind(1, day.date);
    stmt.exec();                    // returns number of rows changed
    transaction.commit();

    //SQLite::Transaction transaction(db);   // RAII: rollback on exception

    //SQLite::Statement stmt(db,
    //    "INSERT INTO people (name, age, height, email) VALUES (?, ?, ?, ?)");

    //for (const auto& p : people) {
    //    stmt.bind(1, p.name);
    //    stmt.bind(2, p.age);
    //    stmt.bind(3, p.height);
    //    stmt.bind(4, p.email.empty() ? nullptr : p.email.c_str());
    //    stmt.exec();
    //    stmt.reset();               // important for reuse
    // Optional: get the generated id
    // int64_t newId = db.getLastInsertRowid();
}

Day DailiesDatabase::findDayByDateString(SQLite::Database& db, std::string dateString)
{
    //SQLite::Statement query(db,
    //    "SELECT id, name, age, height, email FROM people WHERE id = ?");
    SQLite::Statement query(db,
        "SELECT * FROM Days WHERE Date = ?");
    query.bind(1, dateString);

    if (!query.executeStep())
        throw std::runtime_error("Day not found");

    Day day;
    day.id = query.getColumn(0).getInt64();
    day.date = query.getColumn(1).getString();
    return day;
}

void DailiesDatabase::insertTask(SQLite::Database& db, const Day& day, const Task& task)
{
    SQLite::Statement stmt(db,
        "INSERT INTO Tasks (DayId, TaskName, IsDone, CreatedUtc, Type) VALUES (?, ?, ?, ?, ?)");

    SQLite::Transaction transaction(db);
    stmt.bind(1, day.id);
    stmt.bind(2, task.taskName);
    stmt.bind(3, task.isDone);
    stmt.bind(4, task.createdUtc);
    stmt.bind(5, task.type);
    stmt.exec();                    // returns number of rows changed
    transaction.commit();

    //SQLite::Transaction transaction(db);   // RAII: rollback on exception

    //SQLite::Statement stmt(db,
    //    "INSERT INTO people (name, age, height, email) VALUES (?, ?, ?, ?)");

    //for (const auto& p : people) {
    //    stmt.bind(1, p.name);
    //    stmt.bind(2, p.age);
    //    stmt.bind(3, p.height);
    //    stmt.bind(4, p.email.empty() ? nullptr : p.email.c_str());
    //    stmt.exec();
    //    stmt.reset();               // important for reuse
    // Optional: get the generated id
    // int64_t newId = db.getLastInsertRowid();
}