#include <iostream>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <fstream>
#include "Server.h"
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main()
{
    cout << "Starting the server...\n"; 

    // יצירת אובייקט של השרת
    Server server;

    // הפעלת השרת
    if (server.start() != 0) {
        cerr << "Server failed to start!" << endl;
        return 1;
    }

    // השארת השרת פעיל
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}

