//
// Created by pudding on 1/6/26.
//

#pragma once

#include "../infra/sqlite.h"
#include <spdlog/spdlog.h>

#include "../repository/todo/todo.h"

class Container
{
public:
    Container();
    ~Container();
private:
    std::unique_ptr<SQLiteDB> sqlite_con;
    std::unique_ptr<TodosRepository> todos_domain;
};
