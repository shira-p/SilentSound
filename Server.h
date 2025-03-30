#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

class Server {
public:
    Server();
    ~Server();
    int start(); //תפעיל את השרת ותתחיל להאזין לבקשות חיבור מלקוחות תחזיר 0 בהצלחה ן -1 בכישלון
    
    std::vector<std::string> loadUsersFromFile(const std::string& filename); 
    void addUserToFile(const std::string& filename, const std::string& username, const std::string& password);
    bool userExists(const std::string& filename, const std::string& username);

private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    void exitWithError(const std::string& errorMessage);
};

#endif 