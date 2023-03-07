#pragma once

void SendMessage(std::string& sysAlias, TCPSocketClient* client, const sf::IpAddress& ip, unsigned short port, std::string& cliAlias);

void ReadPackage(TCPSocketManager::Status status, sf::Packet& packet, std::string& cliAlias, std::string& sysAlias);
