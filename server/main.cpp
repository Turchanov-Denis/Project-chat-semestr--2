#include <iostream>
#include <string>
#include "httplib.h"
#include "DataBaseManager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ServerManager {
public:
    ServerManager(const std::string& dbFilePath, const std::string& logFile) : databaseManager_(dbFilePath,logFile), logger_(logFile) {
        server_ = std::make_unique<httplib::Server>();
        setupRoutes();
    }

    void start(int port) {
        try {
            if (server_->listen("localhost", port)) {
                std::cout << "Сервер запущен на порту " << port << std::endl;
            } else {
                std::cerr << "Не удалось запустить сервер" << std::endl;
            }
        } catch (const std::exception& ex) {
            std::cerr << "Ошибка при запуске сервера: " << ex.what() << std::endl;
        }
    }

private:
    std::unique_ptr<httplib::Server> server_;
    DatabaseManager databaseManager_;
    Logger logger_;

    static void enableCORS(const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Methods", "GET, POST");
    }

    void getMessages(const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        try {
            json messages = databaseManager_.getMessages();
            res.set_content(messages.dump(), "application/json");
        } catch (const DatabaseException& ex) {
            std::string errorMsg = "Ошибка при получении сообщений из базы данных: " + std::string(ex.what());
            res.set_content(errorMsg, "text/plain");
            logger_.log(errorMsg);
        } catch (const std::exception& ex) {
            std::string errorMsg = "Ошибка при получении сообщений из базы данных: " + std::string(ex.what());
            res.set_content(errorMsg, "text/plain");
            logger_.log(errorMsg);
        }
    }

    void postMessage(const httplib::Request& req, httplib::Response& res) {
        json request = json::parse(req.body);
        std::string message = request["message"];
        std::string characterName = request["characterName"];
        std::string time = request["time"];

        try {
            databaseManager_.addMessage(message, characterName, time);
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content("Сообщение успешно добавлено", "text/plain");
        } catch (const DatabaseException& ex) {
            std::string errorMsg = "Ошибка при добавлении сообщения в базу данных: " + std::string(ex.what());
            res.set_content(errorMsg, "text/plain");
            logger_.log(errorMsg);
        } catch (const std::exception& ex) {
            std::string errorMsg = "Ошибка при добавлении сообщения в базу данных: " + std::string(ex.what());
            res.set_content(errorMsg, "text/plain");
            logger_.log(errorMsg);
        }
    }

    void setupRoutes() {
        server_->Options("/messages", enableCORS);
        server_->Get("/messages", [&](const httplib::Request& req, httplib::Response& res) {
            getMessages(req, res);
        });
        server_->Post("/messages", [&](const httplib::Request& req, httplib::Response& res) {
            postMessage(req, res);
        });
    }
};

int main() {
    try {
        ServerManager serverManager("chat.db", "server.log");
        serverManager.start(8080);
    } catch (const std::exception& ex) {
        std::cerr << "Ошибка при запуске сервера: " << ex.what() << std::endl;
    }

    return 0;
}
