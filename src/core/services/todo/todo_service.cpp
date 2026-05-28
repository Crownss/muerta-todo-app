

#include "todo_service.hpp"

std::optional<TodoItem> TodoService::getById(const std::string &id)
{
    uuid_t uuid;
    if (uuid_parse(id.c_str(), uuid) != 0)
    {
        spdlog::error("[service] Invalid UUID format: {}", id);
        return std::nullopt;
    }
    return repo.getTodoById(uuid);
}

std::vector<TodoItem> TodoService::getAll()
{
    repo.createTable(); // Ensure the table exists before fetching data
    return repo.getAllTodos();
}

bool TodoService::create(const std::string &title, const std::string &description)
{
    TodoItem item;
    uuid_generate(item.id);
    item.title = title;
    item.description = description;
    return repo.createTodo(item);
}

bool TodoService::update(const std::string &id, const std::string &title, const std::string &description, const std::string &status)
{
    uuid_t uuid;
    if (uuid_parse(id.c_str(), uuid) != 0)
    {
        spdlog::error("[service] Invalid UUID format: {}", id);
        return false;
    }
    auto existing = repo.getTodoById(uuid);
    if (!existing)
    {
        spdlog::error("[service] Todo item not found for ID: {}", id);
        return false;
    }
    TodoItem item = existing.value();
    item.title = title;
    item.description = description;
    item.status = status;
    return repo.updateTodo(item);
}

bool TodoService::remove(const std::string &id)
{
    uuid_t uuid;
    if (uuid_parse(id.c_str(), uuid) != 0)
    {
        spdlog::error("[service] Invalid UUID format: {}", id);
        return false;
    }
    return repo.deleteTodoById(uuid);
}