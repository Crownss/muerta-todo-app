//
// Created by pudding on 1/6/26.
//

#include "sqlite.hpp"

SQLiteDB::SQLiteDB() = default;

SQLiteDB::~SQLiteDB()
{
    this->close();
}

bool SQLiteDB::open()
{
    std::string db_path = std::string(getenv("HOME")) + "/.config/muerta/muerta.db";
    std::filesystem::create_directories(std::string(getenv("HOME")) + "/.config/muerta");
    if (sqlite3_open(db_path.c_str(), &this->db_) == SQLITE_OK)
    {
        const bool test_ping = this->test_ping();
        if (!test_ping)
        {
            this->close();
        }
        return test_ping;
    }
    spdlog::error("[infra] Failed to open SQLite DB: {}", sqlite3_errmsg(this->db_));
    this->close();
    return false;
}

void SQLiteDB::close()
{
    if (this->db_ != nullptr)
    {
        sqlite3_close(this->db_);
        this->db_ = nullptr;
    }
}

sqlite3 *SQLiteDB::getHandle() const
{
    if (!this->db_)
    {
        throw std::logic_error("Database not open");
    }
    return this->db_;
}

bool SQLiteDB::test_ping() const
{
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(this->db_, "SELECT 1;", -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        spdlog::error("[infra] Ping failed: {}", sqlite3_errmsg(this->db_));
        return false;
    }
    auto stmtDeleter = [](sqlite3_stmt *s)
    { sqlite3_finalize(s); };
    std::unique_ptr<sqlite3_stmt, decltype(stmtDeleter)> stmtGuard(stmt, stmtDeleter);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        spdlog::error("[infra] Ping step failed: {}", sqlite3_errmsg(this->db_));
        return false;
    }
    spdlog::info("[infra] Opening connection SQLite successful, now is alive!");
    return true;
}
