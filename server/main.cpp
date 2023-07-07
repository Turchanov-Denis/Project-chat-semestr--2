#include <iostream>
#include <string>
#include "httplib.h"
#include "DataBaseManager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


// Управляющий класс сервера
class ServerManager {
public:
    ServerManager(const std::string& dbFilePath) : databaseManager_(dbFilePath) {
        server_ = std::make_unique<httplib::Server>();
        setupRoutes();
    }

    void start(int port) {
        if (server_->listen("localhost", port)) {
            std::cout << "Сервер запущен на порту " << port << std::endl;
        } else {
            std::cerr << "Не удалось запустить сервер" << std::endl;
        }
    }

private:
    std::unique_ptr<httplib::Server> server_;
    DatabaseManager databaseManager_;

    static void enableCORS(const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Methods", "GET, POST");
    }

    void getMessages(const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Content-Type", "application/json");

        json messages = databaseManager_.getMessages();

        res.set_content(messages.dump(), "application/json");
    }

    void postMessage(const httplib::Request& req, httplib::Response& res) {
        json request = json::parse(req.body);
        std::string message = request["message"];
        std::string characterName = request["characterName"];
        std::string time = request["time"];

        databaseManager_.addMessage(message, characterName, time);

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Сообщение успешно добавлено", "text/plain");
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
    ServerManager serverManager("chat.db");
    serverManager.start(8080);

    return 0;
}