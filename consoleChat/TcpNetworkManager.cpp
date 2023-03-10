#include "TcpNetworkManager.h"

TCPSocketManager::Status TCPSocketManager::Send(sf::Packet& packet, sf::IpAddress ip, unsigned short port)
{
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("127.0.0.1", port);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error connecting to " << ip.toString() << ":" << port << std::endl;
		return Status::Error;
	}
	
	status = socket.send(packet);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error sending packet to " << ip.toString() << ":" << port << std::endl;
		return Status::Error;
	}
	
	socket.disconnect();
	return Status::Done;
}

TCPSocketManager::Status TCPSocketManager::Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port)
{
	return Status();
}

unsigned short TCPSocketManager::GetLocalPort()
{
	return 0;
}

#pragma region Server

TCPSocketManager::Status TCPSocketServer::Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port)
{
	sf::Socket::Status status = dispatcher.accept(incoming);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error accepting incoming connection" << std::endl;
		return Status::Error;
	}

	//receive the packet
	status = incoming.receive(packet);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error receiving packet" << std::endl;
		return Status::Error;
	}

	//get the client's IP address and port
	ip = incoming.getRemoteAddress();
	port = incoming.getRemotePort();

	return Status::Done;

}

bool TCPSocketServer::Listen(sf::IpAddress& ip, unsigned short& port)
{
	sf::Socket::Status status = dispatcher.listen(port, "127.0.0.1");

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error listening on port " << port << std::endl;
		return false;
	}

	return true;
}

bool TCPSocketServer::Disconnect()
{
	incoming.disconnect();

	return true;
}

#pragma endregion

#pragma region Client

TCPSocketManager::Status TCPSocketClient::Receive(sf::Packet& packet, sf::IpAddress& ip, unsigned short& port)
{
	sf::Socket::Status status = sock.receive(packet);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error receiving packet" << std::endl;
		return Status::Error;
	}
	
	ip = sock.getRemoteAddress();
	port = sock.getRemotePort();

	return Status::Done;
}

TCPSocketManager::Status TCPSocketClient::Connect(sf::IpAddress& ip, unsigned short port)
{
	sf::Socket::Status status = sock.connect("127.0.0.1", port);

	if (status != sf::Socket::Done)
	{
		std::cerr << "Error connecting to server" << std::endl;
		return Status::Error;
	}

	return Status::Connected;
}

bool TCPSocketClient::Disconnect()
{
	return true;
}

#pragma endregion