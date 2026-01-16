//
// Created by pudding on 1/16/26.
//

#include "todo.h"

TodosRepository::TodosRepository(SQLiteDB& sqlite_con_from_container): sqlite_con(sqlite_con_from_container) {}


bool TodosRepository::createTable()
{
    return true;
}
