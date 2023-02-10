#include "TcpNetworkManager.h"

TCPSockerManager::Status TCPSockerManager::Send(const sf::Packet& packet, sf::IpAddress ip, unsigned short port)
{
	return Status();
}

TCPSockerManager::Status TCPSockerManager::Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port)
{
	return Status();
}

TCPSockerManager::Status TCPSockerManager::Connect(unsigned short port)
{
	return Status();
}

void TCPSockerManager::Disconnect()
{
}

unsigned short TCPSockerManager::GetLocalPort()
{
	return 0;
}
