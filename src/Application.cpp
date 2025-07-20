#include "Application.hpp"
#include <iostream>

int Application::run() {
    auto engine = std::make_shared<Engine>();
    std::cout << "Starting engine..." << std::endl;
    if (!engine->init()) return -1;
    engine->run();
    return 0;
}

