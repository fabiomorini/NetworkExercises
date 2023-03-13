#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include "TcpNetworkManager.h"

constexpr bool debug = false;
constexpr char enterMessage[] = "Enter a message:";
constexpr char waitingClients[] = "Waiting clients...";
constexpr char serverStarted[] = "Server started on ";
constexpr char clientDisconnected[] = "Client status: Disconnected.";
constexpr char clientConnected[] = "Client status: Connected.";
constexpr char clientError[] = "Client status: Error.";
constexpr char clientUnknown[] = "Client status: Unknown.";

//Introducimos una línea de texto por teclado.
std::string GetLineFromCin() {
	std::string line;
	std::getline(std::cin, line);
	return line;
}

//Gestión del envío de paquetes del cliente.
//Al ejecutar esta función, el sistema se quedará bloqueado unicamente enviando mensajes.
void clientMessageHandler(TCPSocketClient* client, sf::IpAddress ip, unsigned short port)
{
    //Creamos un paquete nuevo para enviar el mensaje.
    sf::Packet packet;

    while (true)
    {
        if (debug) std::cout << "System: ";
        std::cout << enterMessage << std::endl;

        packet.clear(); //Limpiamos el paquete para su uso.
        packet << GetLineFromCin();

        client->Send(packet, ip, port);
    }
}

//Gestión del envío de paquetes del servidor.
//Al ejecutar esta función, el sistema se quedará bloqueado unicamente enviando mensajes.
void serverMessageHandler(TCPSocketServer* server, sf::IpAddress ip, unsigned short port)
{
    //Creamos un paquete nuevo para enviar el mensaje.
    sf::Packet packet;

    while (true)
    {
        if (debug) std::cout << "System: ";
        std::cout << enterMessage << std::endl;

        packet.clear(); //Limpiamos el paquete para su uso.
        packet << GetLineFromCin();

        server->Send(packet, ip, port);
    }
}

//Procesamos el paquete para leer el mensaje que contiene.
void packageMessageReader(sf::Packet& packet, std::string& message, std::string& alias)
{
    packet >> message;
    std::cout << alias << message << std::endl;
    packet.clear(); //Limpiamos el paquete.
    message.clear(); //Limpiamos la cadena del mensaje.
    if (debug) std::cout << "System: " << "Recieved package cleared." << std::endl;
}

//Gestión de la conexión de clientes y recepción de paquetes.
//Al ejecutar esta función, el sistema queda a la espera de una conexión entrante.
void serverConnectionsHandler(TCPSocketServer* server, sf::IpAddress ip, unsigned short port)
{
    std::string alias = "Client: ";
    if (server->Listen(ip, port))
    {
        if (debug) std::cout << "System: ";
        std::cout << "Server started on " << ip.toString() << ":" << port << std::endl;

        //Estado de la conexión del socket.
        TCPSocketManager::Status status{};

        do { //Esperamos la conexión de clientes.
            if (debug) std::cout << "System: " << waitingClients << std::endl;

            //Dirección IP del remitente.
            sf::IpAddress remote_ip; //Si nos interesase, aquí podríamos filtrar quien puede enviarnos paquetes.

            //Puerto del remitente.
            unsigned short remote_port;

            //Creamos un paquete nuevo para recuperar la información.
            sf::Packet packet;

            //Receive, para la ejecución del hilo principal.
            status = server->Receive(packet, remote_ip, remote_port);

            //Si hemos obtenido los datos correctamente, obtenemos la información del paquete
            if (status == TCPSocketManager::Status::Done)
            {
                if (debug) std::cout << "System: " << "Client status: Done." << std::endl;

                // Process received packet
                std::string message;
                packageMessageReader(packet, message, alias);
            }
            else {
                switch (status)
                {
                case TCPSocketManager::Status::Disconnected:
                    if (debug) std::cout << "System: ";
                    std::cout << clientDisconnected << std::endl;

                case TCPSocketManager::Status::Connected:
                    if (debug) std::cout << "System: ";
                    std::cout << clientConnected << std::endl;

                case TCPSocketManager::Status::Error:
                    if (debug) std::cout << "System: ";
                    std::cout << clientError << std::endl;

                default:
                    if (debug) std::cout << "System: ";
                    std::cout << clientUnknown << std::endl;
                }
            }

        } while (status != TCPSocketManager::Status::Disconnected); //Si el cliente se desconecta, cerramos la conexión.
    }
    else
    {
        if (debug) std::cout << "System: ";
        std::cout << "Failed to start server." << std::endl;
    }
}

//Gestión de la conexión de clientes y recepción de paquetes.
//Al ejecutar esta función, el sistema queda a la espera de una conexión entrante.
void clientPackageHandler(TCPSocketClient* client, sf::IpAddress ip, unsigned short port)
{
    std::string alias = "Server: ";
    
    //Estado de la conexión del socket.
    TCPSocketManager::Status status{};

    do { //Esperamos la conexión de clientes.
        if (debug) std::cout << "System: " << waitingClients << std::endl;

        //Dirección IP del remitente.
        sf::IpAddress remote_ip; //Si nos interesase, aquí podríamos filtrar quien puede enviarnos paquetes.

        //Puerto del remitente.
        unsigned short remote_port;

        //Creamos un paquete nuevo para recuperar la información.
        sf::Packet packet;

        status = client->Receive(packet, remote_ip, remote_port);

        //Si hemos obtenido los datos correctamente, obtenemos la información del paquete
        if (status == TCPSocketManager::Status::Done)
        {
            // Process received packet
            std::string message;
            packageMessageReader(packet, message, alias);
        }
        else {
            switch (status)
            {
            case TCPSocketManager::Status::Disconnected:
                if (debug) std::cout << "System: ";
                std::cout << clientDisconnected << std::endl;

            case TCPSocketManager::Status::Connected:
                if (debug) std::cout << "System: ";
                std::cout << clientConnected << std::endl;

            case TCPSocketManager::Status::Error:
                if (debug) std::cout << "System: ";
                std::cout << clientError << std::endl;

            default:
                if (debug) std::cout << "System: ";
                std::cout << clientUnknown << std::endl;
            }
        }

    } while (status != TCPSocketManager::Status::Disconnected); //Si el cliente se desconecta, cerramos la conexión.
}

//Selector entre modo cliente o servidor.
int modeSelector()
{
    std::string mode_str;
    if (debug) std::cout << "System: ";
    std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
    std::cin >> mode_str;
    std::cin.clear();
    std::cin.ignore();
    return std::stoi(mode_str);
}

//Gestión del funcionamiento general del servidor.
void serverManager(sf::IpAddress& ip, unsigned short& port)
{
    if (debug) std::cout << "System: ";
    std::cout << "Server mode running." << std::endl;

    //Abrimos un nuevo socket para la comunicación del servidor.
    auto server = new TCPSocketServer();

    serverConnectionsHandler(server, ip, port);
}

//Gestión del funcionamiento general del cliente.
void clientManager(sf::IpAddress& ip, unsigned short& port)
{
    if (debug) std::cout << "System: ";
    std::cout << "Client mode running." << std::endl;

    //Abrimos un nuevo socket para la comunicación del cliente.
    auto client = new TCPSocketClient();

    //Conectamos con el servidor alojado en la dirección y puerto especificados
    if (client->Connect(ip, port) == TCPSocketManager::Status::Connected)
    {
        if (debug) std::cout << "System: ";
        std::cout << "Connected to server." << std::endl;

        //Dejamos que el cliente pueda enviarle mensajes al servidor
        clientMessageHandler(client,ip,port);
    }
}

void main() {
    //Identifica el modo en uso (cliente - servidor).
	int server_mode;
    server_mode = modeSelector();

    //sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::IpAddress ip("127.0.0.1");
    unsigned short port = 5000;

	if (server_mode == 1) {
        serverManager(ip, port);
    }
	else if (server_mode == 2) {
        clientManager(ip, port);
	}
}