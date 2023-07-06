#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Фейковая база данных с сообщениями чата
json chatMessages;

// Обработчик для запроса на получение сообщений чата
void getMessages(const httplib::Request& req, httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Content-Type", "application/json");
    res.set_content(chatMessages.dump(), "application/json");
}

// Обработчик для запроса на добавление сообщения в чат
void postMessage(const httplib::Request& req, httplib::Response& res) {
    json request = json::parse(req.body);

    std::string message = request["message"];

    chatMessages.push_back(message);

    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content("Сообщение успешно добавлено", "text/plain");
}
void enableCORS(const httplib::Request& req, httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.set_header("Access-Control-Allow-Methods", "GET, POST");
}

int main() {
    httplib::Server server;
    server.Options("/messages", enableCORS);
    server.Get("/messages", getMessages);
    server.Post("/messages", postMessage);

    if (server.listen("localhost", 8080)) {
        std::cout << "Сервер запущен на порту 8080" << std::endl;
    } else {
        std::cerr << "Не удалось запустить сервер" << std::endl;
        return -1;
    }

    return 0;
}
