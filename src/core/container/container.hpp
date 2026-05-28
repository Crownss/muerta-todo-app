//
// Created by pudding on 1/6/26.
//

#pragma once

#include "../infra/sqlite.hpp"
#include <spdlog/spdlog.h>

#include "../repository/todo/todo.hpp"
#include "../services/todo/todo_service.hpp"

class Container
{
public:
    Container();
    ~Container();
    TodoService &getTodoService() const { return *todos_service; }

private:
    // infra
    std::unique_ptr<SQLiteDB> sqlite_con;
    // domain
    std::unique_ptr<TodosRepository> todos_domain;
    // service
    std::unique_ptr<TodoService> todos_service;
};
