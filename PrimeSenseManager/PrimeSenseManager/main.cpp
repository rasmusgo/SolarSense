#include "PrimeSense.h"
#include "Network.h"

int main(int argc, char** argv)
{
	PrimeSense primeSense;
	Network network;

	printf("Init done!\n");

	sf::Uint16 opCode;	// 1 - New hand | 2 - Hand lost | 3 - Hand position update | 4 - Swipe right gesture | 5 - Swipe left gesture
	float handX, handY, handZ;

	while (true) {
		// Update PrimeSense
		primeSense.update();

		opCode = 0;
		float handX = 0, handY = 0, handZ = 0;
		// Check if a new hand was found
		if (primeSense.startedTracking()) {
			opCode = 1;
		// Check if the hand was lost
		} else if (primeSense.stoppedTracking()) {
			opCode = 2;
		// Only update position when grabbing
		} else if (primeSense.isGrabbing()) {
			opCode = 3;
			// Get hand position
			primeSense.getHandPosition(&handX, &handY, &handZ);
		}

		// Fill packet with the information
		sf::Packet packet;
		packet << opCode << handX << handY << handZ;

		// Send data
		network.send(packet);
	}
}