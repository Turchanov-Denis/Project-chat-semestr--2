#include <iostream>
#include "httplib.h"
#include <vector>
#include <string>

// Фейковая база данных с сообщениями чата
std::vector<std::string> chatMessages;

// Обработчик для запроса на получение сообщений чата
void getMessages(const httplib::Request& req, httplib::Response& res) {
    std::string response;
    for (const auto& message : chatMessages) {
        response += message + "\n";
    }
    res.set_content(response, "text/plain");

    // Установка заголовков CORS
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
}

// Обработчик для запроса на добавление сообщения в чат
void postMessage(const httplib::Request& req, httplib::Response& res) {
    std::string message = req.body;

    // Добавление сообщения в фейковую базу данных
    chatMessages.push_back(message);

    // Отправка успешного ответа
    res.set_content("Сообщение успешно добавлено", "text/plain");

    // Установка заголовков CORS
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
}

int main() {
    // Создание сервера
    httplib::Server server;

    // Установка маршрутов
    server.Get("/messages", getMessages);
    server.Post("/messages", postMessage);

    // Запуск сервера на порту 8080
    if (server.listen("localhost", 8080)) {
        std::cout << "Сервер запущен на порту 8080" << std::endl;
    } else {
        std::cerr << "Не удалось запустить сервер" << std::endl;
        return -1;
    }

    return 0;
}
