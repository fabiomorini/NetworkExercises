#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include "TcpNetworkManager.h"

void main() {
	int server_mode;
	std::string mode_str;
	std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    unsigned short port = 5000;
    sf::Packet packet;

	if (server_mode == 1) {
        std::cout << "Server mode running";

        auto server = new TCPSocketServer();
        if (server->Listen(ip, port))
        {
            std::cout << "Server started on " << ip.toString() << ":" << port << std::endl;
            // Wait for client to connect
            TCPSocketManager::Status status;
            do {
                sf::IpAddress client_ip;
                unsigned short client_port;
                status = server->Receive(packet, client_ip, client_port);
                if (status == TCPSocketManager::Status::Done)
                {
                    // Process received packet
                    std::string message;
                    packet >> message;
                    std::cout << "Received message from client: " << message << std::endl;
                    packet.clear();
                }
            } while (status != TCPSocketManager::Status::Disconnected);
            std::cout << "Client disconnected" << std::endl;
        }
        else
        {
            std::cout << "Failed to start server" << std::endl;
        }
    }
	else if (server_mode == 2) {
		std::cout << "Client mode running";
        auto client = new TCPSocketClient();
        unsigned short port = 5000;
        if (client->Connect(ip, port) == TCPSocketManager::Status::Connected)
        {
            std::cout << "Connected to server" << std::endl;

            // on sending side
            std::string message = "Client send hello";
            packet << message;
            
            client->Send(packet, ip, port);

            std::cout << "Message sended to server" << std::endl;
        }
	}
}