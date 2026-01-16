#include <spdlog/spdlog.h>

#include "../../core/container/container.h"

int main()
{
    auto cont = std::make_unique<Container>();
    spdlog::info("Hello, World! from Muerta CLI Application.");
    return 0;
}
