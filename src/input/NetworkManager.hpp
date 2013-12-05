#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <SFML/Network.hpp>
#include "VBE/tools.hpp"

class NetworkManager {
public:
    // Singleton method
    static NetworkManager& getInstance() {
        static NetworkManager instance;
        return instance;
    }

    enum Status {
        Working,
        WaitingForConnection,
        Done,
        Error
    };

    enum Gesture {
        None,
        SwipeRight,
        SwipeLeft
    };

    // Creates a connection to a client application
    static void connect();

    // Receive new data from the PrimeSenseManager
    static Status update();

    // Checks if there is a client connected to this manager
    static bool isConnected() { return !waitingForConnection; }

    // Method for checking if a new hand was detected
    static bool isTracking() { return tracking; }

    // Method for checking if the hand is grabbing
    static bool isGrabbing() { return grabbing; }

    // Method for retrieving the hand movement
    static vec3f getHandMovement() { return handMovement; }

    // Checks if a gesture was performed
    static Gesture checkGesture() { return gesture; }

private:
    NetworkManager();
    NetworkManager(NetworkManager const&);
    void operator=(NetworkManager const&);
    ~NetworkManager();

    static int PORT;

    // Network stuff
    static sf::TcpListener listener;
    static sf::TcpSocket client;

    // PrimeSense data
    static vec3f handPos, handMovement;
    static bool waitingForConnection, tracking, grabbing;
    static Gesture gesture;

    static void processPacket(sf::Packet packet);
    static void updateData(int opCode, vec3f newHandPos);
};

#endif // NETWORK_H
