//
// Created by pudding on 1/6/26.
//

#include "container.h"


// Constructor to initialize every heap allocated resources
Container::Container()
{
    if (auto init_sqlite = std::make_unique<SQLiteDB>(); init_sqlite->open())
    {
        sqlite_con = std::move(init_sqlite);
    }
    else
    {
        spdlog::error("Failed to initialize SQLiteDB in Container");
        throw std::runtime_error("Failed to initialize SQLiteDB in Container");
    }
    std::unique_ptr<TodosRepository> init_todos_domain = std::make_unique<TodosRepository>(*sqlite_con);
    spdlog::info("initialized todos_domain");
}

// Destructor to close every heap allocated resources
Container::~Container()
{
    sqlite_con->close();
}
