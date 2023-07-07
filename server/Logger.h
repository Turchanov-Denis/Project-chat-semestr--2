#include <iostream>
#include <fstream>

class Logger {
public:
    Logger(const std::string& logFile) : logFile_(logFile) {
        logStream_.open(logFile_, std::ios::app);
    }

    ~Logger() {
        if (logStream_.is_open()) {
            logStream_.close();
        }
    }

    void log(const std::string& message) {
        logStream_ << message << std::endl;
        logStream_.flush();
    }

private:
    std::ofstream logStream_;
    std::string logFile_;
};