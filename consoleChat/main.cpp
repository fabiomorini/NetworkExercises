#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include "TcpNetworkManager.h"

void main() {
	int server_mode;
	std::string mode_str;
    std::string sysAlias = "System: ";
    std::string srvAlias = "Server: ";
    std::string cliAlias = "Client: ";
	std::cout << sysAlias << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);
    sf::IpAddress ip("127.0.0.1");
    //sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    unsigned short port = 5000;

    #pragma region Server
	if (server_mode == 1) {
        std::cout << sysAlias << "Server mode running." << std::endl;

        auto server = new TCPSocketServer(); //Abrimos un nuevo socket para la comunicación del servidor
        if (server->Listen(ip, port)) //Empezamos a escuchar el puerto especificado si está disponible
        {
            std::cout << sysAlias << "Server started on " << ip.toString() << ":" << port << std::endl;

            // Wait for client to connect
            TCPSocketManager::Status status;

            do { //Esperamos la conexión de clientes
                sf::IpAddress client_ip;
                unsigned short client_port;

                //Creamos un paquete nuevo para que el servidor lo utilice
                sf::Packet packet;

                //Recibimos el paquete y rellenamos las variables creadas anteriormente
                //(obtenemos el ip y puerto del cliente que ha enviado el paquete)
                //(si nos interesase, aquí podríamos filtrar quien puede enviarnos paquetes)
                //Al ejecutar esta línea, el servidor queda a la espera de una conexión entrante de algún cliente
                status = server->Receive(packet, client_ip, client_port);

                std::cout << (status == TCPSocketManager::Status::Done) << std::endl;

                //Si hemos obtenido los datos correctamente, obtenemos la información del paquete
                if (status == TCPSocketManager::Status::Done)
                {
                    // Process received packet
                    std::string message;
                    packet >> message;
                    std::cout << cliAlias << message << std::endl;
                    sf::sleep(sf::milliseconds(1000));
                    packet.clear(); //Limpiamos el paquete
                    std::cout << sysAlias << "Recieved package cleared." << std::endl;
                }
            } while (status != TCPSocketManager::Status::Disconnected); //Si el cliente se desconecta, cerramos la conexión
            std::cout << sysAlias << "Client disconnected." << std::endl;
        }
        else
        {
            std::cout << sysAlias << "Failed to start server." << std::endl;
        }
    }
    #pragma endregion Server

    #pragma region Client
	else if (server_mode == 2) {
		std::cout << sysAlias << "Client mode running." << std::endl;
        auto client = new TCPSocketClient(); //Abrimos un nuevo socket para la comunicación del cliente

        //Conectamos con el servidor alojado en la dirección y puerto especificados
        if (client->Connect(ip, port) == TCPSocketManager::Status::Connected)
        {
            std::cout << sysAlias << "Connected to server." << std::endl;

            //Creamos un paquete nuevo para que el cliente lo utilice
            sf::Packet packet;

            // on sending side
            std::string message = "Client send hello :3";
            packet << message;
            
            client->Send(packet, ip, port); //Enviamos la información de conexión del cliente y la información del paquete a enviar
            packet.clear(); //Limpiamos el paquete
            std::cout << sysAlias << "Sended package cleared." << std::endl;

            std::cout << sysAlias << "Message sended to server." << std::endl;
        }
	}
    #pragma endregion client
}