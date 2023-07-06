#include <iostream>
#include <string>
#include <map>
#include <winsock2.h>

std::map<int, std::map<std::string, std::string>> rooms; // Fake database

std::string processGetRequest(std::string roomId) {
    int id = std::stoi(roomId);
    std::map<std::string, std::string> roomData;

    if (rooms.count(id) > 0) {
        roomData = rooms[id];
    }

    // Convert roomData to JSON string
    std::string jsonResponse = "{\"users\": [";
    for (auto it = roomData.begin(); it != roomData.end(); ++it) {
        jsonResponse += "{\"" + it->first + "\": \"" + it->second + "\"}";
        if (std::next(it) != roomData.end()) {
            jsonResponse += ",";
        }
    }
    jsonResponse += "]}";

    return jsonResponse;
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddress, clientAddress;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create server socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Set server address and port
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(3000);

    // Bind server socket to address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind server socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on server socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port 3000" << std::endl;

    while (true) {
        // Accept incoming connection
        int clientAddressSize = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // Get client IP address
        char clientIPAddress[16];
        strcpy(clientIPAddress, inet_ntoa(clientAddress.sin_addr));
        std::cout << "Client connected from " << clientIPAddress << std::endl;
        std::cout << clientSocket << std::endl;
        // Receive data from client
        const int bufferSize = 4096;
        char buffer[bufferSize];
        int bytesRead = recv(clientSocket, buffer, bufferSize, 0);
        if (bytesRead > 0) {
            // Process the received request
            std::string request(buffer, bytesRead);

            // Check if it is a GET request```cpp
            if (request.find("GET /rooms/") == 0) {
                std::string roomId = request.substr(request.find("/rooms/")+7, request.find("/rooms/")-2);

                std::cout << "Room ID: " << roomId << "all"<< std::endl;
                std::cout << std::stoi( roomId ) << std::endl;
//                std::string roomId = request.substr(12);
//                size_t endPos = request.find(" HTTP/1.1");
//                if (endPos != std::string::npos) {
//                    roomId = request.substr(12, endPos - 12);
//                }
//
                // Process the GET request and generate the response
                std::cout << "ww" << std::endl;
                std::cout << roomId << std::endl;
                std::string response = processGetRequest(roomId);

                // Send the response back to the client
                std::string httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: application/json\r\n";
                httpResponse += "Content-Length: " + std::to_string(response.length()) + "\r\n";
                httpResponse += "\r\n";
                httpResponse += response;

                send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
            }

            // Check if it is a POST request for /rooms
            if (request.find("POST /rooms") == 0) {
                // Extract the room ID and user name from the request
                std::string roomId, userName;
                std::cout << "Post " << request << std::endl;
                size_t roomIdPos = request.find("roomId=");
                size_t userNamePos = request.find("userName=");
                if (roomIdPos != std::string::npos && userNamePos != std::string::npos) {
                    roomId = request.substr(roomIdPos + 7, userNamePos - roomIdPos - 8);
                    userName = request.substr(userNamePos + 9);

                    // Handle the room creation
                    if (!rooms.count(std::stoi(roomId))) {
                        std::map<std::string, std::string> roomData;
                        roomData["users"] = userName;
                        rooms[std::stoi(roomId)] = roomData;
                    }

                    // Send the response back to the client
                    std::string response = "Room created successfully";
                    std::string httpResponse = "HTTP/1.1 200 OK\r\n";
                    httpResponse += "Content-Type: text/plain\r\n";
                    httpResponse += "Content-Length: " + std::to_string(response.length()) + "\r\n";
                    httpResponse += "\r\n";
                    httpResponse += response;

                    send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
                }
            }
        }

        // Close the client socket
        closesocket(clientSocket);
    }

    // Close the server socket and cleanup Winsock
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
