#include "hlotify_logger.h"

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string currentTime = getCurrentTime();
    std::string logMessage = "[" + currentTime + "] [" + logLevelToString(level) + "] " + message;

    if (logFile_.is_open()) {
        logFile_ << logMessage << std::endl;
    }
}

void Logger::info(const std::string& message) {
    log(INFO, message);
}

void Logger::debug(const std::string& message) {
    log(DEBUG, message);
}

void Logger::warning(const std::string& message) {
    log(WARNING, message);
}

    void Logger::error(const std::string& message) {
    log(ERROR, message);
}



void Logger::setLogFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFilePath_ = path;
    logFile_.open(path, std::ios::app);
    if (!logFile_.is_open()) {
        throw std::runtime_error("Failed to open log file: " + path);
    }
}

bool Logger::isTimeInRange(const std::string& time, const std::string& start, const std::string& end) {
    return time >= start && time <= end;
}

void Logger::exportLogs(const std::string& startTime, const std::string& endTime, const std::string& exportPath) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (logFilePath_.empty()) {
        throw std::runtime_error("Log file path is not set.");
    }

    std::ifstream inputFile(logFilePath_);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + logFilePath_);
    }

    std::ofstream exportFile(exportPath);
    if (!exportFile.is_open()) {
        throw std::runtime_error("Failed to open export file: " + exportPath);
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        size_t start = line.find('[');
        size_t end = line.find(']');
        if (start != std::string::npos && end != std::string::npos && end > start) {
            std::string time = line.substr(start + 1, end - start - 1);
            if (isTimeInRange(time, startTime, endTime)) {
                exportFile << line << std::endl;
            }
        }
    }

    inputFile.close();
    exportFile.close();
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S")\
    << std::setw(3) << std::setfill('0');
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}