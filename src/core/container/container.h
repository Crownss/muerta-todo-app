//
// Created by pudding on 1/6/26.
//

#ifndef MUERTA_CONTAINER_H
#define MUERTA_CONTAINER_H
#include "../infra/sqlite.h"
#include <spdlog/spdlog.h>

class Container
{
public:
    Container();
    ~Container();
private:
    SQLiteDB* sqlite_con = nullptr;
};
#endif //MUERTA_CONTAINER_H