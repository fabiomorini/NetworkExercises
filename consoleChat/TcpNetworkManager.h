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
	virtual Status Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port) override;
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