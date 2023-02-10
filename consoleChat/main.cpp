#pragma once
#include <SFML\Network.hpp>
#include <iostream> 

void main() {
	int server_mode;
	std::string mode_str;
	std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);

	if (server_mode == 1) {
		std::cout << "Server mode running";
		// server connect
		sf::TcpListener dispatcher;
		sf::Socket::Status status = dispatcher.listen(5000);
		if (status != sf::Socket::Done)
		{
			//No se puede vincular al puerto 5000
		}

		sf::TcpSocket incoming;
		//Al conectarse un cliente, el socket incoming pasa a ser el que utilizan este cliente y el servidor para comunicarse en exclusiva
		if (dispatcher.accept(incoming) != sf::Socket::Done)
		{
			//Error al aceptar conexión
		}
	}
	else if (server_mode == 2) {
		std::cout << "Client mode running";
		sf::TcpSocket socket;
		sf::Socket::Status status = socket.connect("127.0.0.1", 5000, sf::seconds(5.f));
		if (status != sf::Socket::Done)
		{
			//No se ha podido conectar
		}


	}
}