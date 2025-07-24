#include "Application.hpp"
#include "Logger.hpp"

int Application::run() {
    Logger::set_lvl(LogLevel::DEBUG);
    auto engine = std::make_shared<Engine>();
    Logger::log("Starting engine...", LogLevel::INFO);
    if (!engine->init()) return -1;
    engine->run();
    return 0;
}

