#include <iostream>
#include <spdlog/spdlog.h>
#include <uuid/uuid.h>
#include <string>

int main()
{
    uuid_t randomized_uuid;
    uuid_generate_random(randomized_uuid);
    std::string uuid_str(36, '\0');
    uuid_unparse(randomized_uuid, uuid_str.data());
    uuid_parse(uuid_str.c_str(), randomized_uuid);
    spdlog::info("Hello, World! {}", uuid_str);

    return 0;
}