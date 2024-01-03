//
// Created by jmisa on 3. 1. 2024.
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WS2tcpip.h>
#include <windows.h>

#ifndef WILDFIRE_SIMULATION_POS_SERVER_H
#define WILDFIRE_SIMULATION_POS_SERVER_H


class Server {
private:
    SOCKET serverSocket;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    std::vector<std::string> mapy;

public:
    Server() : serverSocket(-1), clientSocket(-1), clientAddrLen(sizeof(clientAddr)) {}

    bool setup(int port) {
        // Vytvorenie socketu pre server
        WSADATA Data;
        WSAStartup(MAKEWORD(2, 2), &Data);
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Chyba pri vytvarani socketu" << std::endl;
            return false;
        }

        // Nastavenie adriesy a portu pre server
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(port);

        // Pripojenie socketu na danú adresu a port
        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Chyba pri bindovaní socketu" << std::endl;
            closesocket(serverSocket);
            return false;
        }

        // Počúvanie na sockete
        if (listen(serverSocket, 10) == -1) {
            std::cerr << "Chyba pri počúvaní na sockete" << std::endl;
            closesocket(serverSocket);
            return false;
        }

        std::cout << "Server čaká na pripojenie..." << std::endl;
        return true;
    }

    bool acceptConnection() {
        // Akceptovanie pripojenia
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Chyba pri akceptovaní pripojenia" << std::endl;
            closesocket(serverSocket);
            return false;
        }

        std::cout << "Klient pripojený" << std::endl;
        return true;
    }

    bool receiveMessage() {
        // Pripravenie buffra pre prijímanie správ od klienta
        char buffer[1024] = {0};
        // Prijímanie správy od klienta
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Chyba pri prijímaní správy od klienta" << std::endl;
            closesocket(serverSocket);
            closesocket(clientSocket);
            return false;
        }

        std::string mapa = buffer;
        mapy.push_back(mapa);

        std::cout << "Klient poslal správu:\n" << mapy[mapy.size() -1 ] << std::endl;

        // Odpovedanie klientovi
        const char *response = "Správa prijatá!";
        if (send(clientSocket, response, strlen(response), 0) == -1) {
            std::cerr << "Chyba pri odosielaní odpovede klientovi" << std::endl;
            closesocket(serverSocket);
            closesocket(clientSocket);
            return false;
        }

        return true;
    }

    void closeConnection() {
        // Uzavretie socketov
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        std::cout << "Ukoncene servera" << std::endl;
    }

    void startServer(Server *server) {
        while(true){
            server->acceptConnection();
            server->receiveMessage();
        }
    }
};


#endif //WILDFIRE_SIMULATION_POS_SERVER_H
