#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include "TcpNetworkManager.h"
#include <future>


std::string GetLineFromCin() {
	std::string line;

	std::getline(std::cin, line);
    std::cin.clear();

	return line;
}

void clientMessageHandler(TCPSocketClient* client, sf::IpAddress ip, unsigned short port)
{
    std::string cliAlias = "Client: ";

    //Creamos un paquete y cadena de texto nuevos para enviar el mensaje
    sf::Packet packet;
    std::string message;

    while (true)
    {
        //El cliente escribe un mensaje
        std::cout << "System: " << "Enter a message:" << std::endl;

        message = GetLineFromCin();

        packet << message;

        client->Send(packet, ip, port); //Enviamos la información de conexión y la información del paquete a enviar
        std::cout << cliAlias << message << std::endl;

        packet.clear(); //Limpiamos el paquete
        message.clear(); //Limpiamos la cadena para el mensaje
    }
}

void serverMessageHandler(TCPSocketServer* server, sf::IpAddress ip, unsigned short port)
{
    std::string srvAlias = "Server: ";

    //Creamos un paquete y cadena de texto nuevos para enviar el mensaje
    sf::Packet packet;
    std::string message;

    while (true)
    {
        if (message.size() < 0)
        {
            //El cliente escribe un mensaje
            std::cout << "System: " << "Enter a message:" << std::endl;

            std::getline(std::cin, message);
        }
        else
        {
            packet << message;

            server->Send(packet, ip, port); //Enviamos la información de conexión y la información del paquete a enviar
            std::cout << srvAlias << message << std::endl;

            message.clear(); //Limpiamos la cadena para el mensaje
            packet.clear(); //Limpiamos el paquete
        }
    }
}

void serverPackageHandler(TCPSocketServer* server)
{
    TCPSocketManager::Status status;

    do { //Esperamos la conexión de clientes
        std::cout << "System: " << "Waiting clients..." << std::endl;

        //Recibimos el paquete y rellenamos las variables creadas anteriormente
        //(obtenemos el ip y puerto del emisor del paquete)
        //(si nos interesase, aquí podríamos filtrar quien puede enviarnos paquetes)
        //Al ejecutar esta línea, el sistema queda a la espera de una conexión entrante
        std::string alias = "Client: ";
        sf::IpAddress client_ip;
        unsigned short client_port;

        //Creamos un paquete nuevo para recuperar la información
        sf::Packet packet;

        status = server->Receive(packet, client_ip, client_port);

        //Si hemos obtenido los datos correctamente, obtenemos la información del paquete
        if (status == TCPSocketManager::Status::Done)
        {
            // Process received packet
            std::string message;
            packet >> message;
            std::cout << alias << message << std::endl;
            packet.clear(); //Limpiamos el paquete
            std::cout << "System: " << "Recieved package cleared." << std::endl;
        }
        else {
            switch (status)
            {
            case TCPSocketManager::Status::Disconnected:
                std::cout << "System: " << "Client status: Disconnected." << std::endl;

            case TCPSocketManager::Status::Connected:
                std::cout << "System: " << "Client status: Connected." << std::endl;

            case TCPSocketManager::Status::Error:
                std::cout << "System: " << "Client status: Error." << std::endl;

            default:
                std::cout << "System: " << "Client status: Unknown." << std::endl;
            }
        }

    } while (status != TCPSocketManager::Status::Disconnected); //Si el cliente se desconecta, cerramos la conexión
}

void clientPackageHandler(TCPSocketClient* client)
{
    TCPSocketManager::Status status;

    do { //Esperamos la conexión de clientes
        std::cout << "System: " << "Waiting clients..." << std::endl;

        //Recibimos el paquete y rellenamos las variables creadas anteriormente
        //(obtenemos el ip y puerto del emisor del paquete)
        //(si nos interesase, aquí podríamos filtrar quien puede enviarnos paquetes)
        //Al ejecutar esta línea, el sistema queda a la espera de una conexión entrante
        std::string alias = "Server: ";
        sf::IpAddress client_ip;
        unsigned short client_port;

        //Creamos un paquete nuevo para recuperar la información
        sf::Packet packet;

        status = client->Receive(packet, client_ip, client_port);

        //Si hemos obtenido los datos correctamente, obtenemos la información del paquete
        if (status == TCPSocketManager::Status::Done)
        {
            // Process received packet
            std::string message;
            packet >> message;
            std::cout << alias << message << std::endl;
            packet.clear(); //Limpiamos el paquete
            std::cout << "System: " << "Recieved package cleared." << std::endl;
        }
        else {
            switch (status)
            {
            case TCPSocketManager::Status::Disconnected:
                std::cout << "System: " << "Client status: Disconnected." << std::endl;

            case TCPSocketManager::Status::Connected:
                std::cout << "System: " << "Client status: Connected." << std::endl;

            case TCPSocketManager::Status::Error:
                std::cout << "System: " << "Client status: Error." << std::endl;

            default:
                std::cout << "System: " << "Client status: Unknown." << std::endl;
            }
        }
    } while (status != TCPSocketManager::Status::Disconnected); //Si el cliente se desconecta, cerramos la conexión
}

void main() {
	int server_mode;
	std::string mode_str;
	std::cout << "System: " << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
    std::cin.clear();
    std::cin.ignore();
	server_mode = std::stoi(mode_str);
    sf::IpAddress ip("127.0.0.1");
    //sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    unsigned short port = 5000;

    #pragma region Server
	if (server_mode == 1) {
        std::cout << "System: " << "Server mode running." << std::endl;

        auto server = new TCPSocketServer(); //Abrimos un nuevo socket para la comunicación del servidor

        //Dejamos que el servidor pueda enviar mensajes a los clientes
        //std::future<void> serverMessengerHandler = std::async(std::launch::async, serverMessageHandler, server, ip, port);

        if (server->Listen(ip, port)) //Empezamos a escuchar el puerto especificado si está disponible
        {
            std::cout << "System: " << "Server started on " << ip.toString() << ":" << port << std::endl;

            //Estado de la conexión con el cliente
            TCPSocketManager::Status status;

            std::future<void> serverPackagesHandler = std::async(std::launch::async, serverPackageHandler, server);
        }
        else
        {
            std::cout << "System: " << "Failed to start server." << std::endl;
        }
    }
    #pragma endregion Server

    #pragma region Client
	else if (server_mode == 2) {
		std::cout << "System: " << "Client mode running." << std::endl;
        auto client = new TCPSocketClient(); //Abrimos un nuevo socket para la comunicación del cliente

        //Conectamos con el servidor alojado en la dirección y puerto especificados
        if (client->Connect(ip, port) == TCPSocketManager::Status::Connected)
        {
            std::cout << "System: " << "Connected to server." << std::endl;

            //Dejamos que el cliente pueda enviarle mensajes al servidor
            std::future<void> clientMessengerHandler = std::async(std::launch::async, clientMessageHandler, client, ip, port);
        }
	}
    #pragma endregion client
}