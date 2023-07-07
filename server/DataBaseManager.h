#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <sqlite/sqlite3.h>
#include "Logger.h"
using json = nlohmann::json;

// Простой класс логгера


// Исключение для ошибок базы данных
class DatabaseException : public std::exception {
public:
    explicit DatabaseException(const std::string& errorMsg) : errorMsg_(errorMsg) {}

    const char* what() const noexcept override {
        return errorMsg_.c_str();
    }

private:
    std::string errorMsg_;
};

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbFilePath, const std::string& logFile) : db_(nullptr), logger_(logFile) {
        int rc = sqlite3_open(dbFilePath.c_str(), &db_);
        if (rc != SQLITE_OK) {
            std::string errorMsg = "Не удалось открыть базу данных: ";
            errorMsg += sqlite3_errmsg(db_);
            logger_.log(errorMsg);
            throw DatabaseException(errorMsg);
        }
    }

    ~DatabaseManager() {
        if (db_) {
            sqlite3_close(db_);
        }
    }

    json getMessages() {
        json messages;

        std::string sql = "SELECT * FROM messages";
        char* errorMsg = nullptr;

        int rc = sqlite3_exec(db_, sql.c_str(), selectCallback, &messages, &errorMsg);
        if (rc != SQLITE_OK) {
            std::string errorMsgStr = "Ошибка выполнения запроса: ";
            errorMsgStr += errorMsg;
            logger_.log(errorMsgStr);
            sqlite3_free(errorMsg);
            throw DatabaseException(errorMsgStr);
        }

        return messages;
    }

    void addMessage(const std::string& message, const std::string& characterName, const std::string& time) {
        std::string sql = "INSERT INTO messages (message, characterName, time) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::string errorMsg = "Ошибка при подготовке запроса: ";
            errorMsg += sqlite3_errmsg(db_);
            logger_.log(errorMsg);
            throw DatabaseException(errorMsg);
        }

        sqlite3_bind_text(stmt, 1, message.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, characterName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, time.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::string errorMsg = "Ошибка при выполнении запроса: ";
            errorMsg += sqlite3_errmsg(db_);
            logger_.log(errorMsg);
            sqlite3_finalize(stmt);
            throw DatabaseException(errorMsg);
        }

        sqlite3_finalize(stmt);
    }

private:
    sqlite3* db_;
    Logger logger_;

    static int selectCallback(void* data, int argc, char** argv, char** azColName) {
        json* jsonData = static_cast<json*>(data);

        json row;
        for (int i = 0; i < argc; i++) {
            row[azColName[i]] = argv[i] ? argv[i] : "";
        }

        jsonData->push_back(row);

        return 0;
    }
};
