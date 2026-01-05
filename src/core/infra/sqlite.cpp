//
// Created by pudding on 1/6/26.
//

#include "sqlite.h"

SQLiteDB::SQLiteDB() = default;

SQLiteDB::~SQLiteDB() {
    close();
}

bool SQLiteDB::open() {
    if (sqlite3_open("./muerta.db", &db_) == SQLITE_OK) {
        return test_ping();
    }
    spdlog::error("Failed to open SQLite DB: {}", sqlite3_errmsg(db_));
    close();
    return false;
}

void SQLiteDB::close() {
    if (db_ != nullptr)
    {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

sqlite3* SQLiteDB::getHandle() const {
    return db_;
}


bool SQLiteDB::test_ping() const
{
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db_, "SELECT 1;", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        spdlog::error("Ping failed: {}", sqlite3_errmsg(db_));
        return false;
    }
    auto stmtDeleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    std::unique_ptr<sqlite3_stmt, decltype(stmtDeleter)> stmtGuard(stmt, stmtDeleter);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        spdlog::error("Ping step failed: {}", sqlite3_errmsg(db_));
        return false;
    }
    spdlog::info("Opening connection SQLite successful, now is alive!");
    return true;
}
