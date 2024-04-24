#include "NetworkManager.h"
#include <string>

NetworkManager::NetworkManager() {}

//Cerrar socket y liberar recursos
NetworkManager::~NetworkManager() {

	closesocket(this->socketServer);
	this->socketClient = INVALID_SOCKET;
	WSACleanup();
}

SOCKET NetworkManager::GetClientSocket() {

	return this->socketClient;
}

bool NetworkManager::StartNetworkService() {

	WSADATA wsData;
	WORD ver = MAKEWORD(MAJOR_VERSION_WINSOCK, MINOR_VERSION_WINSOCK);

	// Inicializar Winsock
	if (WSAStartup(ver, &wsData) == 0) {

		//Crear socket
		this->socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (this->socketServer != INVALID_SOCKET) {

			//Configuramos parametros de red
			this->serverSocketAddressData.sin_family = AF_INET; //IPV4
			this->serverSocketAddressData.sin_port = htons(PORT); //htons para big endians ntons/sin funcion para little endians
			this->serverSocketAddressData.sin_addr.S_un.S_addr = INADDR_ANY; //Aceptar conexiones de cualquier IP

			//Vinculamos socket con ip y puerto
			bind(this->socketServer, (sockaddr*)&this->serverSocketAddressData, sizeof(this->serverSocketAddressData));

			//Esperamos conexion
			listen(this->socketServer, MAX_CONNECTIONS);

			std::cout << "Esperando conexion." << std::endl;

			return true;
		}
		else {
			std::cerr << "Error al inicializar el socket. Codigo de error: " << WSAGetLastError() << std::endl;
			return false;
		}
	}
	else {
		std::cerr << "Error al inicializar Winsock, codigo de error: " << WSAGetLastError() << std::endl;
		return false;
	}
}

bool NetworkManager::AcceptConnection() {

	int clientSize = sizeof(this->clientSocketAddressData);

	//Acepta conexion
	this->socketClient = accept(this->socketServer, (sockaddr*)&this->clientSocketAddressData, &clientSize);

	//Conexion satisfactoria. Conversion de IP binaria a formato string para debugar origen
	if (this->socketClient != INVALID_SOCKET) {

		char clientIp[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &this->clientSocketAddressData.sin_addr, clientIp, INET_ADDRSTRLEN);

		std::cout << "Conexion establecida con: " << clientIp << std::endl;
		return true;

	}
	else {
		std::cerr << "No se pudo aceptar la conexion" << std::endl;
		return false;
	}
}

void NetworkManager::EndConnection() {
	closesocket(this->socketClient);
	this->socketClient = INVALID_SOCKET;
	WSACleanup();
}

std::string NetworkManager::ReceiveData() {

	std::string data = "";
	int bytesReceived = 0;
	int totalBytesReceived = 0;

	memset(this->buffer, 0, BUFFER_SIZE);

	std::cout << "Esperando informacion." << std::endl;

	//-1 = Fallo en la conexion, 0 = cliente finaliza conexion o recibidos valores nulos
	while ((bytesReceived = recv(this->socketClient, this->buffer, BUFFER_SIZE, 0)) > 0) {
		
		totalBytesReceived += bytesReceived - 1;
		data += this->buffer;

		//Mirar si esta toda la info usando el delimitador y retornarla
		if (data[totalBytesReceived - 1] == DELIMITER) {
			return data;
		}
		//Si no se ha recibido el delimitador limpiamos buffer hasta que llegue la faltante
		else {
			memset(this->buffer, 0, BUFFER_SIZE);
		}
	}

	this->EndConnection();
	return "-1";
}

