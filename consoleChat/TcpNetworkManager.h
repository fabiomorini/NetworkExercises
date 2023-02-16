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
	Status Send(sf::Packet& packet, sf::IpAddress ip, unsigned short port);
	virtual Status Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port) = 0;
	unsigned short GetLocalPort();
	virtual bool Disconnect() = 0;
};

class TCPSocketServer : public TCPSocketManager
{
	sf::TcpListener dispatcher;
	sf::TcpSocket incoming;
	sf::TcpSocket sock;

public:

	virtual Status Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port);
	bool Listen(sf::IpAddress& ip, unsigned short& port);
	virtual bool Disconnect();
};

class TCPSocketClient : public TCPSocketManager
{
	sf::TcpSocket sock;

public:
	virtual Status Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port);
	Status Connect(unsigned short port);
};