#include <iostream>
#include <string>
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <sqlite/sqlite3.h>

using json = nlohmann::json;

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbFilePath) : db_(nullptr) {
        int rc = sqlite3_open(dbFilePath.c_str(), &db_);
        if (rc != SQLITE_OK) {
            std::cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db_) << std::endl;
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
            std::cerr << "Ошибка выполнения запроса: " << errorMsg << std::endl;
            sqlite3_free(errorMsg);
        }

        return messages;
    }

    void addMessage(const std::string& message, const std::string& characterName, const std::string& time) {
        std::string sql = "INSERT INTO messages (message, characterName, time) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Ошибка при подготовке запроса: " << sqlite3_errmsg(db_) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, message.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, characterName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, time.c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Ошибка при выполнении запроса: " << sqlite3_errmsg(db_) << std::endl;
            sqlite3_finalize(stmt);
            return;
        }

        sqlite3_finalize(stmt);
    }

private:
    sqlite3* db_;

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
