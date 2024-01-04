//
// Created by jmisa on 3. 1. 2024.
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WS2tcpip.h>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>

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
    bool exit = false;
    int i = 0;
public:
    Server() : serverSocket(-1), clientSocket(-1), clientAddrLen(sizeof(clientAddr)) {}

    bool setup(int port) {
        // Vytvorenie socketu pre server
        this->nacitajSvety();
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
        if (clientSocket == -1 && this->exit) {
            //std::cerr << "ukoncenie serveraa" << std::endl;
            return false;
        } else if (clientSocket == -1) {
            closesocket(serverSocket);
            return false;
        }

        std::cout << "Klient pripojeny" << std::endl;
        return true;
    }

    bool receiveMessage() {
        // Pripravenie buffra pre prijímanie správ od klienta
        char buffer[1024] = {0};
        // Prijímanie správy od klienta
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (this->exit && bytesReceived == -1) {
            //std::cerr << "ukoncenie serveraa" << std::endl;
            return false;
        } else if (bytesReceived == -1) {
            closesocket(serverSocket);
            closesocket(clientSocket);
            return false;
        }
        std::string volba = buffer;
        std::istringstream stream(volba);
        int num = 0;
        for (std::string line; std::getline(stream, line);)
            num++;

        if (num <= 2) {
            std::string nazovMapy = buffer;

            std::istringstream stream2(nazovMapy);
            std::string nazov;
            std::getline(stream2, nazov);

            std::cout << "Klient poslal spravu:\n" << nazov << std::endl;
            // Odpovedanie klientovi vybranu mapu
            const char *response = "";
            for (int i = 0; i < this->mapy.size(); ++i) {
                std::istringstream streamMapy(this->mapy[i]);
                std::string nazovTemp;
                std::getline(streamMapy, nazovTemp);

                if (nazov == nazovTemp) {
                    response = this->mapy[i].c_str();
                    break;
                }
            }
            if (strlen(response) == 0) {
                response = "subor neexistuje";
                if (send(clientSocket, response, strlen(response), 0) == -1) {
                    std::cerr << "Chyba pri odosielani odpovede klientovi" << std::endl;
                    closesocket(serverSocket);
                    closesocket(clientSocket);
                    return false;
                }
                return true;
            }
            if (send(clientSocket, response, strlen(response), 0) == -1) {
                std::cerr << "Chyba pri odosielani odpovede klientovi" << std::endl;
                closesocket(serverSocket);
                closesocket(clientSocket);
                return false;
            }
            return true;
        } else {
            std::string mapa = buffer;

            std::istringstream stream2(mapa);
            std::string nazov;
            std::getline(stream2, nazov);

            if (this->SvetJeNaServeri(nazov)) {
                std::cout << "Klient poslal spravu:\n" << mapa << std::endl;
                // Odpovedanie klientovi ze subot uz existuje
                const char *response = "subor existuje";
                if (send(clientSocket, response, strlen(response), 0) == -1) {
                    std::cerr << "Chyba pri odosielani odpovede klientovi" << std::endl;
                    closesocket(serverSocket);
                    closesocket(clientSocket);
                    return false;
                }
                return true;
            } else {
                mapy.push_back(mapa);
                mapa += "\n@\n";
                this->SvetUlozNaServer(mapa);

                std::cout << "Klient poslal spravu:\n" << mapy[mapy.size() - 1] << std::endl;

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
        }
        return true;
    }

    void SvetUlozNaServer(const std::string &svet) {
        std::ofstream subor("worldsServer.txt", std::ios::app);
        if (!subor) {
            std::cerr << "Nastala chyba pri otvarani suboru" << std::endl;
            return;
        }

        subor << svet;

        subor.close();
    }

    bool SvetJeNaServeri(const std::string &nazovSvetu) {
        for (int i = 0; i < this->mapy.size(); ++i) {
            std::istringstream stream(this->mapy[i]);
            std::string nazov;
            std::getline(stream, nazov);

            if (nazov == nazovSvetu) {
                return true;
            }
        }
        return false;
    }

    void nacitajSvety() {
        std::ifstream subor;
        subor.open("worldsServer.txt");

        std::string svet;

        if (subor.is_open()) {
            while (subor) {
                std::string riadok;
                std::getline(subor, riadok);
                if (riadok == "@") {
                    this->mapy.push_back(svet);
                    svet = "";
                } else {
                    svet += riadok + "\n";
                }
            }
        }
    }

    void closeConnection() {
        // Uzavretie socketov
        exit = true;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        std::cout << "Ukoncenie servera" << std::endl;
    }

    void startServer(Server *server) {
        while(!server->exit) {
            server->acceptConnection();
            bool check = server->receiveMessage();
            if (!check) {
                break;
            }
            server->i++;
        }
    }

    void setExit(bool exitPar) {
        this->exit = exitPar;
    }

    bool getExit() {
        return this->exit;
    }
};


#endif //WILDFIRE_SIMULATION_POS_SERVER_H
