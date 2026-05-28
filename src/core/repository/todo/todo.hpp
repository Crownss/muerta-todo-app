//
// Created by pudding on 1/16/26.
//

#pragma once

#include "../../infra/sqlite.hpp"
#include "models.hpp"
#include <optional>
#include <vector>
#include "../../helper/tz.hpp"

class TodosRepository
{
public:
    explicit TodosRepository(SQLiteDB &sqlite_con_from_container): sqlite_con(sqlite_con_from_container) {}

    bool createTable(); // Initialize schema
    bool createTodo(const TodoItem &item);
    std::optional<TodoItem> getTodoById(const uuid_t &id);
    std::vector<TodoItem> getAllTodos();
    bool updateTodo(const TodoItem &item);
    bool deleteTodoById(const uuid_t &id);
    // ~TodosRepository();

    // ~TodosDomain();
private:
    SQLiteDB &sqlite_con;
    std::string table_name = "todos";

    // for check result and logging
    bool checkResult(int rc, const char *msg) const
    {
        if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW)
        {
            spdlog::error("{}: {}", msg, sqlite3_errmsg(sqlite_con.getHandle()));
            return false;
        }
        return true;
    }
};