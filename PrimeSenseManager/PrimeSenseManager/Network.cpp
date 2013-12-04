//#include "StdAfx.h"
#include "Network.h"

Network::Network() {
	// Connect to server
	sf::Socket::Status status = socket.connect(IP_ADDRESS, PORT);
	if (status != sf::Socket::Done)	{
		printf("Error while connecting.\n");
	}
}

Network::~Network() {}

// Send data to the client
void Network::send(sf::Packet& packet) {
	// TCP socket:
	sf::Socket::Status status = socket.send(packet);
	if (status != sf::Socket::Done) {
		printf("Error while sending package.\n");
	}
}

// Send data to the client
void Network::receive(sf::Packet& packet) {
	socket.receive(packet);

	std::string msg;
	packet >> msg;
}