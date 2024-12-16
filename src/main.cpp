#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sys/prctl.h>

const std::string PID_FILE = "/tmp/hlotify.pid";
const std::string LOG_FILE = "/tmp/hlotify.log";

class Logger {
public:
    enum Level { DEBUG, INFO, ERROR };

    Logger(const std::string& file) {
        log_stream.open(file, std::ios::app);
        if (!log_stream) {
            throw std::runtime_error("Failed to open log file: " + file);
        }
    }

    ~Logger() {
        if (log_stream.is_open()) {
            log_stream.close();
        }
    }

    void log(const std::string& message, Level level) {
        if (log_stream.is_open()) {
            log_stream << timestamp() << " [" << level_to_string(level) << "] " << message << std::endl;
        }
    }

private:
    std::ofstream log_stream;

    std::string timestamp() {
        std::time_t now = std::time(nullptr);
        std::tm tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string level_to_string(Level level) {
        switch (level) {
            case DEBUG: return "DEBUG";
            case INFO: return "INFO";
            case ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }
};

Logger logger(LOG_FILE);

pid_t read_pid() {
    std::ifstream pid_file(PID_FILE);
    pid_t pid;
    if (pid_file >> pid) {
        logger.log("Read PID from file: " + std::to_string(pid), Logger::DEBUG);
        return pid;
    }
    logger.log("No PID found in file.", Logger::DEBUG);
    return -1;
}

void write_pid(pid_t pid) {
    std::ofstream pid_file(PID_FILE);
    if (pid_file) {
        pid_file << pid;
        logger.log("Wrote PID to file: " + std::to_string(pid), Logger::DEBUG);
    } else {
        logger.log("Failed to write PID to file.", Logger::ERROR);
    }
}

void remove_pid_file() {
    if (std::remove(PID_FILE.c_str()) == 0) {
        logger.log("Removed PID file.", Logger::DEBUG);
    } else {
        logger.log("Failed to remove PID file.", Logger::ERROR);
    }
}

bool is_process_running(pid_t pid) {
    if (kill(pid, 0) == 0) {
        logger.log("Process with PID " + std::to_string(pid) + " is running.", Logger::DEBUG);
        return true;
    } else {
        if (errno == ESRCH) {
            logger.log("No process with PID " + std::to_string(pid) + " found.", Logger::DEBUG);
        } else {
            logger.log("Error checking PID " + std::to_string(pid) + ": " + std::strerror(errno), Logger::ERROR);
        }
        return false;
    }
}

void daemon_work() {
    while (true) {
        sleep(5);
        logger.log("Daemon is running...", Logger::DEBUG);
    }
}

void start_daemon() {
    logger.log("Attempting to start daemon...", Logger::DEBUG);
    std::cout << "Starting daemon..." << std::endl;

    pid_t existing_pid = read_pid();
    if (existing_pid > 0) {
        if (is_process_running(existing_pid)) {
            logger.log("Daemon is already running with PID: " + std::to_string(existing_pid), Logger::ERROR);
            std::cout << "Daemon is already running with PID: " << existing_pid << std::endl;
            return;
        } else {
            logger.log("Stale PID file detected. Removing PID file.", Logger::INFO);
            remove_pid_file();
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        logger.log("Failed to fork.", Logger::ERROR);
        std::cerr << "Failed to fork." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        logger.log("Daemon started with PID: " + std::to_string(pid), Logger::INFO);
        std::cout << "Daemon started with PID: " << pid << std::endl;
        return;
    }

    logger.log("Child process created for daemon.", Logger::DEBUG);

    if (setsid() < 0) {
        logger.log("Failed to create new session.", Logger::ERROR);
        std::cerr << "Failed to create new session." << std::endl;
        exit(EXIT_FAILURE);
    }

    logger.log("New session created.", Logger::DEBUG);

    if (prctl(PR_SET_NAME, "hlotify-daemon", 0, 0, 0) != 0) {
        logger.log("Failed to set process name.", Logger::ERROR);
        exit(EXIT_FAILURE);
    }
    logger.log("Process name set to 'hlotify-daemon'.", Logger::INFO);

    if (!freopen("/dev/null", "r", stdin)) {
        logger.log("Failed to redirect stdin.", Logger::ERROR);
    }
    if (!freopen("/dev/null", "w", stdout)) {
        logger.log("Failed to redirect stdout.", Logger::ERROR);
    }
    if (!freopen("/dev/null", "w", stderr)) {
        logger.log("Failed to redirect stderr.", Logger::ERROR);
    }

    write_pid(getpid());

    signal(SIGTERM, [](int) {
        logger.log("Daemon is stopping...", Logger::INFO);
        remove_pid_file();
        exit(EXIT_SUCCESS);
    });

    daemon_work();
}

void stop_daemon() {
    logger.log("Attempting to stop daemon...", Logger::DEBUG);
    std::cout << "Stopping daemon..." << std::endl;

    pid_t pid = read_pid();
    if (pid > 0 && is_process_running(pid)) {
        if (kill(pid, SIGTERM) == 0) {
            logger.log("Daemon with PID: " + std::to_string(pid) + " stopped.", Logger::INFO);
            std::cout << "Daemon with PID: " << pid << " stopped." << std::endl;
        } else {
            logger.log("Failed to send SIGTERM to PID: " + std::to_string(pid), Logger::ERROR);
            std::cerr << "Failed to send SIGTERM to PID: " << pid << std::endl;
        }
    } else {
        logger.log("No running daemon found.", Logger::ERROR);
        std::cerr << "No running daemon found." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            logger.log("Usage: " + std::string(argv[0]) + " start|stop", Logger::ERROR);
            std::cerr << "Usage: " << argv[0] << " start|stop" << std::endl;
            return EXIT_FAILURE;
        }

        std::string command = argv[1];

        logger.log("Command received: " + command, Logger::DEBUG);

        if (command == "start") {
            start_daemon();
        } else if (command == "stop") {
            stop_daemon();
        } else {
            logger.log("Unknown command: " + command, Logger::ERROR);
            std::cerr << "Unknown command: " << command << std::endl;
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
