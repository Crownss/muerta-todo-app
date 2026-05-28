// todo_service.hpp
#pragma once
#include "../../repository/todo/todo.hpp"

class TodoService
{
    TodosRepository &repo;

public:
    explicit TodoService(TodosRepository &r) : repo(r)
    {
        r.createTable();
    }
    std::optional<TodoItem> getById(const std::string &id);
    std::vector<TodoItem> getAll();
    bool create(const std::string &title, const std::string &description);
    bool update(const std::string &id, const std::string &title, const std::string &description, const std::string &status);
    bool remove(const std::string &id);
};