#include "Server.h"
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

//פונקציה שמקבלת שם של קובץ ומחזירה את רשימת המשתמשים מתוך הקובץ
std::vector<std::string> Server::loadUsersFromFile(const std::string& filename) {

    std::vector<std::string> users;
    nlohmann::json j;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return users;  // מחזיר רשימה ריקה במקרה של טעות
    }

    file >> j;  // קריאת הנתונים מהקובץ
    if (!j.is_null()) {
        for (const auto& user : j["users"]) {
            users.push_back(user["username"]);  // הוספת שם המשתמש לרשימה
        }
    }

    return users;  // מחזיר את רשימת המשתמשים
}

// פונקציה להוספת משתמש לקובץ JSON
void Server::addUserToFile(const std::string& filename, const std::string& username, const std::string& password) {
    nlohmann::json j;

    // קריאה של קובץ JSON
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> j;  // קריאה לתוכן הקובץ
    }

    // הוספת משתמש חדש
    nlohmann::json newUser = {
        {"username", username},
        {"password", password}
    };

    j["users"].push_back(newUser);

    // שמירה של הקובץ מחדש
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << j.dump(4);  // שומר את הנתונים בקובץ עם אינדנטציה
    } else {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
    }
}

// פונקציה לבדוק אם משתמש קיים בקובץ JSON
bool Server::userExists(const std::string& filename, const std::string& username) {
    std::vector<std::string> users = loadUsersFromFile(filename);
    return std::find(users.begin(), users.end(), username) != users.end();
}


Server::Server() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        exitWithError("WSAStartup failed");
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        exitWithError("Socket creation failed");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
}

Server::~Server() {
    closesocket(serverSocket);
    WSACleanup();
}

int Server::start() {
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        exitWithError("Bind failed");
        return 1;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        exitWithError("Listen failed");
        return 1;
    }

    std::cout << "Server listening on port..." << std::endl;
    //// מחכה ללחיצה על ENTER כדי שהשרת לא ייסגר מיידית
    //std::cout << "Server is running. Press ENTER to exit..." << std::endl;
    //std::cin.get();


    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client connected!" << std::endl;

        // קריאת הבקשה מהלקוח
        char buffer[1024] = {0};
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::string request(buffer, bytesReceived);
            std::cout << "Received request:\n" << request << std::endl;

            // בדיקה אם זו בקשת POST ל- /api/login
            if (request.find("POST /api/login") != std::string::npos) {
                // מציאת גוף ה-JSON בבקשה
                size_t jsonStart = request.find("\r\n\r\n") + 4;
                std::string jsonData = request.substr(jsonStart);
                
                std::vector<std::string> users = loadUsersFromFile("users.json");
                
                // בדיקה אם השם משתמש והסיסמה נכונים (לשם דוגמה)
                if (jsonData.find("\"username\":\"testuser\"") != std::string::npos &&
                    jsonData.find("\"password\":\"1234\"") != std::string::npos) {
                    
                    std::string httpResponse = 
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "Access-Control-Allow-Origin: *\r\n"
                        "Content-Length: 27\r\n\r\n"
                        "{\"message\":\"Login successful\"}";

                    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
                } else {
                    std::string httpResponse = 
                        "HTTP/1.1 401 Unauthorized\r\n"
                        "Content-Type: application/json\r\n"
                        "Access-Control-Allow-Origin: *\r\n"
                        "Content-Length: 28\r\n\r\n"
                        "{\"message\":\"Invalid credentials\"}";

                    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
                }
            }
        }

        closesocket(clientSocket);
    }

    return 0;
}

void Server::exitWithError(const std::string& errorMessage) {
    std::cerr << errorMessage << std::endl;
    closesocket(serverSocket);
    WSACleanup();
    exit(EXIT_FAILURE);
}
