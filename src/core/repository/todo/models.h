//
// Created by pudding on 1/16/26.
//
// todo_item.h
#pragma once
#include <string>
#include <optional>
#include <uuid/uuid.h>

struct TodoItem {
    uuid_t id;
    std::string title;
    std::string description;
    std::string status; // "pending", "in_progress", "completed"

    bool isValid() const { return !title.empty() && !uuid_is_null(id); }
    // Ensure status is set to "pending" if empty
    void ensureStatus() {
        if (status.empty()) status = "pending";
    }
};