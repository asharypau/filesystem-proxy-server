#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

#include "colors.h"
#include "datetimeprovider.h"

class Logger
{
public:
    template<class... Args>
    static void debug(const std::format_string<Args...>& fmt, Args&&... args)
    {
        std::cout << Colors::BRIGHT_BLUE << "[DEBUG]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t'
                  << std::format(fmt, std::forward<Args>(args)...) << Colors::RESET << std::endl;
    }

    template<class... Args>
    static void info(const std::format_string<Args...>& fmt, Args&&... args)
    {
        std::cout << Colors::GREEN << "[INFO]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t'
                  << std::format(fmt, std::forward<Args>(args)...) << Colors::RESET << std::endl;
    }

    template<class... Args>
    static void warning(const std::format_string<Args...>& fmt, Args&&... args)
    {
        std::cout << Colors::YELLOW << "[WARN]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t'
                  << std::format(fmt, std::forward<Args>(args)...) << Colors::RESET << std::endl;
    }

    template<class... Args>
    static void error(const std::format_string<Args...>& fmt, Args&&... args)
    {
        std::cout << Colors::RED << "[ERROR]\t" << Colors::WHITE << DateTimeProvider::utc_now() << '\t'
                  << std::format(fmt, std::forward<Args>(args)...) << Colors::RESET << std::endl;
    }
};

#endif
