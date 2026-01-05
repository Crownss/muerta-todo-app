//
// Created by pudding on 1/6/26.
//

#include "container.h"


// Constructor to initialize every heap allocated resources
Container::Container()
{
    if (auto init_sqlite = std::make_unique<SQLiteDB>(); init_sqlite->open())
    {
        sqlite_con = init_sqlite.release();
    }
}

// Destructor to close every heap allocated resources
Container::~Container()
{
    sqlite_con->close();
}
