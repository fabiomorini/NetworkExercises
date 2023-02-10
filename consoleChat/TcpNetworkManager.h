#pragma once
#include <SFML\Network.hpp>
class TCPSockerManager {
	sf::TcpSocket sock;
public:
	enum class Status
	{
		Done, ///< The socket has sent / received the data correctly
		Error, ///< An unexpected error happened
		Connected, ///< The socket is connected and ready to work
		Disconnected, ///< The TCP socket is disconnected
	};
	///< Methods to implement
	Status Send(const sf::Packet& packet, sf::IpAddress ip, unsigned short port);
	Status Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port);
	Status Connect(unsigned short port);
	void Disconnect();
	unsigned short GetLocalPort();
};