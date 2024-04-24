#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#define MAJOR_VERSION_WINSOCK 2
#define MINOR_VERSION_WINSOCK 2
#define MAX_CONNECTIONS 1
#define PORT 8080
#define BUFFER_SIZE 2048
#define DELIMITER '@'

#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class NetworkManager {
private:

    SOCKET socketServer;
    SOCKET socketClient;

    sockaddr_in serverSocketAddressData;
    sockaddr_in clientSocketAddressData;

    char buffer[BUFFER_SIZE];

public:
    NetworkManager();
    ~NetworkManager();

    bool StartNetworkService();
    bool AcceptConnection();
    void EndConnection();
    std::string ReceiveData();
    SOCKET GetClientSocket();
};

#endif
