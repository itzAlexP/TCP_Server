#include <iostream>
#include <WS2tcpip.h>
#include "NetworkManager.h"

#pragma comment(lib, "ws2_32.lib")

void main() {

	bool serverOn = true;
	NetworkManager networkManager = NetworkManager::NetworkManager();

	//Iniciamos servicio de red
	if (networkManager.StartNetworkService()) {

		//Espera a recibir una conexion
		if (networkManager.AcceptConnection()) {
			while (networkManager.GetClientSocket() != INVALID_SOCKET) {

				//Edu aqui recibes el paquete entero con el @incluido al final. Si recibes un -1 es que la conexion se ha caido por algun lado
				networkManager.ReceiveData();
			}
		}
	}
}
