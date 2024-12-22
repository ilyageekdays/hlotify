#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <stdexcept>

class Logger {
public:
    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static Logger& getInstance();
    void setLogFile(const std::string& path);
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);

    // Экспорт логов из файла
    void exportLogs(const std::string& startTime, const std::string& endTime, const std::string& exportPath);

private:
    Logger() = default;
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getCurrentTime();
    std::string logLevelToString(LogLevel level);

    // Проверка времени на принадлежность диапазону
    bool isTimeInRange(const std::string& time, const std::string& start, const std::string& end);

    std::ofstream logFile_;
    std::string logFilePath_;
    std::mutex mutex_;
};

#endif // LOGGER_H
