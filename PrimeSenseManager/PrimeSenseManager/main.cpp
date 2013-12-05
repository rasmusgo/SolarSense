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
		PrimeSense::Gesture gesture = primeSense.getGesture();

		opCode = 0;
		float handX = 0, handY = 0, handZ = 0;
		// Check we stopped tracking the user
		if (primeSense.startedTracking()) {
			opCode = 1;
		// Check if we stopped tracking the user
		} else if (primeSense.stoppedTracking()) {
			opCode = 2;
		// Only update position when grabbing
		} else if (primeSense.isGrabbing()) {
			opCode = 3;
			// Get hand position
			primeSense.getHandPosition(&handX, &handY, &handZ);
		} else {
			switch (primeSense.getGesture()) {
			case PrimeSense::SWIPE_RIGHT:
				opCode = 4;
				break;
			case PrimeSense::SWIPE_LEFT:
				opCode = 5;
				break;
			}
		}

		// Fill packet with the information
		sf::Packet packet;
		packet << opCode << handX << handY << handZ;

		// Send data
		network.send(packet);
	}
}