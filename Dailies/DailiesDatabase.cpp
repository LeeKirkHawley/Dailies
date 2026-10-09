#include "pch.h"
#include "DailiesDatabase.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <stdexcept>
#include <string>

void DailiesDatabase::Migrate(SQLite::Database& db)
{
    try
    {
        SQLite::Transaction tx(db);
        int version = db.execAndGet("PRAGMA user_version").getInt();

        if (version < 1)
        {
            db.exec(R"(
                CREATE TABLE IF NOT EXISTS Project (
                    Id INTEGER PRIMARY KEY,
                    Task TEXT NOT NULL,
                    CreatedUtc TEXT NOT NULL
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
        Migrate(db);
        return db;
    }
    catch (const std::exception& ex)
    {
        throw winrt::hresult_error(E_FAIL, winrt::to_hstring(ex.what()));
    }
}
