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
            std::cerr << "Chyba pri bindovani socketu" << std::endl;
            closesocket(serverSocket);
            return false;
        }

        // Počúvanie na sockete
        if (listen(serverSocket, 10) == -1) {
            std::cerr << "Chyba pri pocuvani na sockete" << std::endl;
            closesocket(serverSocket);
            return false;
        }

        std::cout << "Server caka na pripojenie..." << std::endl;
        return true;
    }

    bool acceptConnection() {
        // Akceptovanie pripojenia
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Chyba pri akceptovani pripojenia" << std::endl;
            closesocket(serverSocket);
            return false;
        }

        std::cout << "Klient pripojeny" << std::endl;
        return true;
    }

    bool receiveMessage() {
        char buffer[1024] = {0};

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Chyba pri prijimani spravy od klienta" << std::endl;
            closesocket(serverSocket);
            closesocket(clientSocket);
            return false;
        }

        std::cout << "Vytvorena mapa:\n" << buffer << std::endl;

        // Odpovedanie klientovi
        const char *response = "Sprava prijata!";
        if (send(clientSocket, response, strlen(response), 0) == -1) {
            std::cerr << "Chyba pri odosielani odpovede klientovi" << std::endl;
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
        std::cout << "Ukoncenie serveru" << std::endl;
    }

    void startServer(Server *server) {
        server->setup(8080);
        server->acceptConnection();
    }
};


#endif //WILDFIRE_SIMULATION_POS_SERVER_H
