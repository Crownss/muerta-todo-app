#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

std::string toTimestamp(std::chrono::system_clock::time_point tp);
std::chrono::system_clock::time_point fromTimestamp(const std::string &s);