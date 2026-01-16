//
// Created by pudding on 1/16/26.
//

#pragma once

#include "../../infra/sqlite.h"

class TodosRepository
{
public:
    explicit TodosRepository(SQLiteDB& sqlite_con_from_container);


    bool createTable(); // Initialize schema


    // ~TodosDomain();
private:
    SQLiteDB& sqlite_con;

    // for check result and logging
    bool checkResult(int rc, const char* msg) const {
        if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW) {
            spdlog::error("{}: {}", msg, sqlite3_errmsg(sqlite_con.getHandle()));
            return false;
        }
        return true;
    }
};