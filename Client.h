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
    std::string svetTemp;

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

    int sendMessage(const char *message) {
        this->svetTemp = "";
        // Poslanie správy serveru
        if (send(clientSocket, message, strlen(message), 0) == -1) {
            std::cerr << "Chyba pri odosielani spravy" << std::endl;
            closesocket(clientSocket);
            return -1;
        }
        // Pripravenie buffra pre prijímanie odpovede od servera
        char buffer[1024] = {0};

        // Prijímanie odpovede od servera
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Chyba pri prijimani odpovede od servera" << std::endl;
            closesocket(clientSocket);
            return -1;
        }

        std::cout << "Server odpovedal: " << buffer << std::endl;

        std::string odpoved(buffer);
        if (odpoved == "subor existuje") {
            return 0;
        } else if (odpoved == "subor neexistuje") {
            return -2;
        } else if (odpoved != "Sprava prijata!") {
            this->svetTemp = odpoved;
            return 1;
        }

        return 1;
    }

    std::string vratSvet() {
        if (this->svetTemp.empty()) {
            return "chyba";
        } else {
            return this->svetTemp;
        }
    }

    void closeConnection() {
        // Uzavretie socketu klienta
        closesocket(clientSocket);
    }
};

#endif //WILDFIRE_SIMULATION_POS_CLIENT_H
