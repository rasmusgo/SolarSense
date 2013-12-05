#include "NetworkManager.hpp"

int NetworkManager::PORT = 50536;

// Network stuff
sf::TcpListener NetworkManager::listener;
sf::TcpSocket NetworkManager::client;

// PrimeSense data
vec3f NetworkManager::handPos(0,0,0), NetworkManager::handMovement(0,0,0);
bool NetworkManager::waitingForConnection(true), NetworkManager::tracking(false), NetworkManager::grabbing(false);
NetworkManager::Gesture NetworkManager::gesture(NetworkManager::None);

NetworkManager::NetworkManager() {}
NetworkManager::~NetworkManager() {}

void NetworkManager::connect() {
    std::cout << "Network | Initialise network..." << std::endl;
    listener.setBlocking(false);
    sf::Socket::Status status;

    // Bind the listener to a port
    status = listener.listen(PORT);
    if (status == sf::Socket::Done) {
        std::cout << "Network | Error while binding." <<std::endl;
        return;
    }

    /*// Accept incoming connection
    status = listener.accept(client);

    if (status == sf::Socket::NotReady) {
        waitingForConnection = true;
        std::cout << "Network | Waiting for incoming connection attempts..." << std::endl;
        return;
    } else if (status == sf::Socket::Error) {
        std::cout << "Network | Error while accepting connection." << std::endl;
        return;
    }*/

    std::cout << "Network | Initialisation complete." << std::endl;
}

NetworkManager::Status NetworkManager::update() {
    sf::Socket::Status status;
    if (waitingForConnection) {
        status = listener.accept(client);
        if (status == sf::Socket::NotReady) {
            std::cout << "Network | Waiting for incoming connection attempts..." << std::endl;
        } else if (status == sf::Socket::Done) {
            waitingForConnection = false;
            std::cout << "Network | Connection established." << std::endl;
        } else if (status == sf::Socket::Error) {
            std::cout << "Network | Error while accepting connection." << std::endl;
            return NetworkManager::Error;;
        }

        return NetworkManager::WaitingForConnection;
    } else {

        // Get the data from client with movement and gesture information
        sf::Packet packet;
        status = client.receive(packet);
        if (status == sf::Socket::Done) {
            // Packet received
            processPacket(packet);
        } else if (status == sf::Socket::Disconnected) {
            // Lost connection to client
            std::cout << "Network | Connection to client lost." << std::endl;
            waitingForConnection = true;
        } else if (status == sf::Socket::Error) {
            return NetworkManager::Error;
        }

        /*// Unpacking received packet
        sf::Uint16 opCode;
        float velX, velY, velZ;
        if (packet >> opCode >> velX >> velY >> velZ) {
            updateData(opCode, vec3f(velX, velY, velZ));
        } else {
            return NetworkManager::Error;
        }*/

        return NetworkManager::Done;
    }
}

void NetworkManager::processPacket(sf::Packet packet) {
    // Unpacking received packet
    sf::Uint16 opCode;
    float velX, velY, velZ;
    if (packet >> opCode >> velX >> velY >> velZ) {
        updateData(opCode, vec3f(velX, velY, velZ));
    } else {
        std::cout << "Network | Error unpacking packet." << std::endl;
    }
}

void NetworkManager::updateData(int opCode, vec3f newHandPos) {
    vec3f aux;
    // Reset gesture
    gesture = None;

    switch (opCode) {
    case 1: // New hand detected
        tracking = true;
        break;
    case 2: // Hand lost
        tracking = false;
        break;
    case 3: // Hand position update
        // Set hand position for the first frame grabbing is detected
        if (handPos.length() < 0.1f || !grabbing)
            handPos = newHandPos;

        aux = newHandPos - handPos;
        aux = vec3f(aux.y, aux.x, aux.z);
        handMovement = 0.9f*handMovement + 0.1f*aux;
        //handMovement = newHandPos - handPos;
        handPos = newHandPos;
        // Correct hand movement.
        //handMovement = vec3f(handMovement.y, -handMovement.x, -handMovement.z);
        break;
    case 4: // Swiping right gesture
        gesture = SwipeRight;
        break;
    case 5: // Swiping left gesture
        gesture = SwipeLeft;
        break;
    }

    grabbing = opCode == 3;
}
