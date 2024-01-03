//
// Created by jmisa on 3. 1. 2024.
//

#ifndef WILDFIRE_SIMULATION_POS_CLIENT_H
#define WILDFIRE_SIMULATION_POS_CLIENT_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WS2tcpip.h>
#include <windows.h>

class Client {
private:
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;

public:
    Client() : clientSocket(-1) {}

    bool connectToServer(const char *serverIP, int port) {
        // Vytvorenie socketu pre klienta
        int wsaStatus;
        WSADATA WSAData;
        wsaStatus=WSAStartup(MAKEWORD(2, 0), &WSAData);
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Chyba pri vytvarani socketu" << std::endl;
            return false;
        }

        // Nastavenie adriesy a portu servera, ku ktorému sa chceme pripojiť
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

        if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
            std::cerr << "Neplatna adresa servera" << std::endl;
            closesocket(clientSocket);
            return false;
        }

        // Pripojenie k serveru
        if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Chyba pri pripajani k serveru" << std::endl;
            closesocket(clientSocket);
            return false;
        }

        std::cout << "Pripojeny k serveru" << std::endl;
        return true;
    }

    bool sendMessage(const char *message) {
        // Poslanie správy serveru
        if (send(clientSocket, message, strlen(message), 0) == -1) {
            std::cerr << "Chyba pri odosielani spravy" << std::endl;
            closesocket(clientSocket);
            return false;
        }
        // Pripravenie buffra pre prijímanie odpovede od servera
        char buffer[1024] = {0};

        // Prijímanie odpovede od servera
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Chyba pri prijimani odpovede od servera" << std::endl;
            closesocket(clientSocket);
            return false;
        }

        std::cout << "Server odpovedal: " << buffer << std::endl;

        return true;
    }

    void closeConnection() {
        // Uzavretie socketu klienta
        closesocket(clientSocket);
    }
};

#endif //WILDFIRE_SIMULATION_POS_CLIENT_H
