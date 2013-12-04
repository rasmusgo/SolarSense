#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>

#define IP_ADDRESS "192.168.62.131" 
#define PORT 50536 

class Network {
public:
	Network();
	~Network();

	// Send data to the server
	void send(sf::Packet& packet);
	// Receive data from the server
	void receive(sf::Packet& packet);

private:
	sf::TcpSocket socket;
};

#endif // NETWORK_H