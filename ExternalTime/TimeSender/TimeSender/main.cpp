#include <iostream>
#include <string>
#include <sstream>
#include <SFML\System.hpp>
#include <SFML\Network.hpp>

#define DEFAULT_PORT        4242

void SendTime(sf::Clock& clock) {

}

int main() {
	sf::Clock clock;
	sf::TcpListener listener;

	if (listener.listen(DEFAULT_PORT) != sf::Socket::Done)
	{
		std::cout << "Error creating listener at port " << DEFAULT_PORT << std::endl;
		return -1;
	}

	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << "Error accepting client" << std::endl;
		return -1;
	}
	clock.restart();

	// Connection established
	sf::Packet inP;
	while (1) {
		client.receive(inP);

		sf::Packet outP;
		outP << clock.getElapsedTime().asSeconds();

		client.send(outP);
	}

	return 42;
}