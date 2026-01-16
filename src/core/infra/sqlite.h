//
// Created by pudding on 1/6/26.
//

#pragma once

#include <memory>
#include <sqlite3.h>
#include <spdlog/spdlog.h>

class SQLiteDB {
public:
    SQLiteDB();
    ~SQLiteDB();
    bool open();
    void close();
    sqlite3* getHandle() const;
private:
    bool test_ping() const;
    sqlite3* db_ = nullptr;
};