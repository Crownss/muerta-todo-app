//
// Created by pudding on 1/16/26.
//
// todo_item.h
#pragma once
#include <string>
#include <uuid/uuid.h>
#include <chrono>

struct TodoItem
{
    uuid_t id;
    std::string title;
    std::string description;
    std::string status = "pending"; // "pending", "in_progress", "completed"
    std::chrono::system_clock::time_point created_at = std::chrono::system_clock::now();

    bool isValid() const { return !title.empty() && !uuid_is_null(id); }
};