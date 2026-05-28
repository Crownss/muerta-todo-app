//
// Created by pudding on 1/16/26.
//

#include "todo.hpp"

bool TodosRepository::createTable()
{
    std::string table_schema = R"((
            id TEXT PRIMARY KEY,
            title TEXT NOT NULL,
            description TEXT,
            status TEXT NOT NULL DEFAULT 'pending',
            created_at TEXT NOT NULL
        );
    )";
    const std::string query = "CREATE TABLE IF NOT EXISTS " + this->table_name + table_schema;

    int rc = sqlite3_exec(this->sqlite_con.getHandle(), query.c_str(), nullptr, nullptr, nullptr);
    return checkResult(rc, "[repository] Failed to create todos table");
}

bool TodosRepository::createTodo(const TodoItem &item)
{
    const std::string query = "INSERT INTO " + this->table_name + " (id, title, description, status, created_at) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->sqlite_con.getHandle(), query.c_str(), -1, &stmt, nullptr);
    if (!checkResult(rc, "[repository] Failed to prepare statement for creating todo"))
    {
        return false;
    }
    char id_str[37];
    uuid_unparse(item.id, id_str);
    sqlite3_bind_text(stmt, 1, id_str, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, item.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, item.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, item.status.c_str(), -1, SQLITE_TRANSIENT);
    std::string created_at_str = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(item.created_at.time_since_epoch()).count());
    sqlite3_bind_text(stmt, 5, created_at_str.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return checkResult(rc, "[repository] Failed to execute statement for creating todo");
}

std::optional<TodoItem> TodosRepository::getTodoById(const uuid_t &id)
{
    const std::string query = "SELECT id, title, description, status, created_at FROM " + this->table_name + " WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->sqlite_con.getHandle(), query.c_str(), -1, &stmt, nullptr);

    if (!checkResult(rc, "[repository] Failed to prepare statement for getting todo by id"))
    {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }
    char uuid_str[37];
    uuid_unparse(id, uuid_str);
    sqlite3_bind_text(stmt, 1, uuid_str, -1, SQLITE_STATIC);

    TodoItem todo;
    if (checkResult(rc, "[repository] Failed to execute statement for getting todo by id") && sqlite3_step(stmt) == SQLITE_ROW)
    {
        uuid_parse(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)), todo.id);
        todo.title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        todo.description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        todo.status = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        todo.created_at = fromTimestamp(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));

        sqlite3_finalize(stmt);
        return todo;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

bool TodosRepository::updateTodo(const TodoItem &item)
{
    std::optional<TodoItem> existing = getTodoById(item.id);
    char id_str[37];
    uuid_unparse(item.id, id_str);
    if (!existing.has_value())
    {
        spdlog::error("Todo with id {} not found for update", id_str);
        return false;
    }

    const std::string title = item.title.empty() ? existing->title : item.title;
    const std::string description = item.description.empty() ? existing->description : item.description;
    const std::string status = item.status.empty() ? existing->status : item.status;

    if (title == existing->title && description == existing->description && status == existing->status)
    {
        spdlog::info("[repository] No changes detected, skipping update");
        return true;
    }

    const std::string query = "UPDATE " + this->table_name + " SET title = ?, description = ?, status = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->sqlite_con.getHandle(), query.c_str(), -1, &stmt, nullptr);
    if (!checkResult(rc, "[repository] Failed to prepare statement for updating todo"))
        return false;

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, id_str, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return checkResult(rc, "[repository] Failed to execute statement for updating todo");
}

bool TodosRepository::deleteTodoById(const uuid_t &id)
{
    const std::string query = "DELETE FROM " + this->table_name + " WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->sqlite_con.getHandle(), query.c_str(), -1, &stmt, nullptr);
    if (!checkResult(rc, "[repository] Failed to prepare statement for deleting todo"))
    {
        return false;
    }

    char uuid_str[37];
    uuid_unparse(id, uuid_str);
    sqlite3_bind_text(stmt, 1, uuid_str, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return checkResult(rc, "[repository] Failed to execute statement for deleting todo");
}

std::vector<TodoItem> TodosRepository::getAllTodos()
{
    const std::string query = "SELECT id, title, description, status, created_at FROM " + this->table_name + ";";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->sqlite_con.getHandle(), query.c_str(), -1, &stmt, nullptr);

    std::vector<TodoItem> todos;
    if (!checkResult(rc, "[repository] Failed to prepare statement for getting all todos"))
    {
        sqlite3_finalize(stmt);
        return todos;
    }

    while (checkResult(rc, "[repository] Failed to execute statement for getting all todos") && sqlite3_step(stmt) == SQLITE_ROW)
    {
        TodoItem todo;
        uuid_parse(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)), todo.id);
        todo.title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        todo.description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        todo.status = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        todo.created_at = fromTimestamp(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
        todos.push_back(todo);
    }

    sqlite3_finalize(stmt);
    return todos;
}