#pragma once
#include <SFML\Network.hpp>
#include <iostream>

class TCPSocketManager {
	
public:
	enum class Status
	{
		Done, ///< The socket has sent / received the data correctly
		Error, ///< An unexpected error happened
		Connected, ///< The socket is connected and ready to work
		Disconnected, ///< The TCP socket is disconnected
	};

	TCPSocketManager() = default;
	
	//Enviamos la información de conexión y la información del paquete a enviar.
	Status Send(sf::Packet& packet, sf::IpAddress ip, unsigned short port);
	
	virtual Status Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port) = 0;
	unsigned short GetLocalPort();
	virtual bool Disconnect() = 0;
	std::string sysAlias = "System: ";
};

class TCPSocketServer : public TCPSocketManager
{
	sf::TcpListener dispatcher;
	sf::TcpSocket incoming;
	sf::TcpSocket sock;

public:
	TCPSocketServer() = default;

	//Recepción de paquetes e información del remitente.
	virtual Status Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port) override;
	
	//Empezamos a escuchar el puerto especificado si está disponible.
	bool Listen(sf::IpAddress& ip, unsigned short& port);
	
	virtual bool Disconnect() override;
};

class TCPSocketClient : public TCPSocketManager
{
	sf::TcpSocket sock;

public:
	TCPSocketClient() = default;
	virtual Status Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port) override;
	Status Connect(sf::IpAddress& ip, unsigned short port);
	virtual bool Disconnect() override;
};