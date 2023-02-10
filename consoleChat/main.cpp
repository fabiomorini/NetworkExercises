#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include "TcpNetworkManager.h"

const std::string IP = "127.0.0.1";
unsigned short PORT = 5000;

void main() {
	int server_mode;
	std::string mode_str;
	std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);


	if (server_mode == 1) {
		std::cout << "Server mode running";
		TCPSocketServer* server = new TCPSocketServer;
		sf::Packet* packet;
		sf::IpAddress ip = sf::IpAddress(IP);

		server->Listen(ip, PORT);
		while (true)
		{
			server->Receive(packet, ip, PORT);
			//if message.size > 0 , send, message.clear();
		}
				
	}
	else if (server_mode == 2) {
		std::cout << "Client mode running";
		TCPSocketClient* client;

		sf::TcpSocket socket;
		sf::Socket::Status status = socket.connect("127.0.0.1", 5000, sf::seconds(5.f));
		if (status != sf::Socket::Done)
		{
			//No se ha podido conectar
		}


	}
}