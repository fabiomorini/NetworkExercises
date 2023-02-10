#include "TcpNetworkManager.h"

TCPSocketManager::Status TCPSocketManager::Send(sf::Packet& packet, sf::IpAddress ip, unsigned short port)
{
	return Status();
}

TCPSocketManager::Status TCPSocketManager::Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port)
{
	return Status();
}

unsigned short TCPSocketManager::GetLocalPort()
{
	return 0;
}

bool TCPSocketManager::Disconnect()
{
}

#pragma region Server

TCPSocketManager::Status TCPSocketServer::Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port)
{
	sf::Packet received_packet;
	std::string message;
	incoming.receive(received_packet); received_packet >> message;

}

bool TCPSocketServer::Listen(sf::IpAddress& ip, unsigned short& port)
{
	sf::Socket::Status status = dispatcher.listen(port, ip);
	if (status == sf::Socket::Error)
	{
		std::cout << "No se puede vincular al puerto "<< port << std::endl;
		return false;
	}

	if (dispatcher.accept(incoming) == sf::Socket::Error)
	{
		std::cout << "Error al aceptar la conexión" << std::endl;
		return false;
	}

	//crear socket

	return true;
}

bool TCPSocketServer::Disconnect()
{
	
}

#pragma endregion

#pragma region Client

TCPSocketManager::Status TCPSocketClient::Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port)
{
	return Status();
}

TCPSocketManager::Status TCPSocketClient::Connect(unsigned short port)
{
	return Status();
}

#pragma endregion