#include "Planet.hpp"

Planet::Planet(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed)
        : GameObject(parentScene, parentObject->pos, scale)
        , orbRadius(orbRadius), orbSpeed(orbSpeed), timeAcc(0.0f) {
    std::cout << orbRadius << std::endl;
}

Planet::~Planet(){
}
