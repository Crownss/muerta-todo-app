//
// Created by pudding on 1/6/26.
//

#include "container.hpp"

// Constructor to initialize every heap allocated resources
Container::Container()
{
    if (std::unique_ptr<SQLiteDB> init_sqlite = std::make_unique<SQLiteDB>(); init_sqlite->open())
    {
        this->sqlite_con = std::move(init_sqlite);
    }
    else
    {
        std::string error_msg = "Failed to open SQLiteDB in Container: " + std::string(this->sqlite_con->getHandle() ? sqlite3_errmsg(this->sqlite_con->getHandle()) : "Unknown error");
        spdlog::error(error_msg);
        throw std::runtime_error(error_msg);
    }
    std::unique_ptr<TodosRepository> init_todos_domain = std::make_unique<TodosRepository>(*this->sqlite_con);
    this->todos_domain = std::move(init_todos_domain);
    spdlog::info("initialized todos_domain");
    std::unique_ptr<TodoService> init_todos_service = std::make_unique<TodoService>(*this->todos_domain);
    this->todos_service = std::move(init_todos_service);
    spdlog::info("initialized todos_service");
}

// Destructor to close every heap allocated resources
Container::~Container()
{
    this->sqlite_con->close();
}
