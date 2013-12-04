#include "Planet.hpp"
#include "Camera.hpp"

Planet::Planet(const std::string& name, float radius, float orbRadius)
    : radius(radius), orbRadius(orbRadius), orbSpeed(0.0f), drawOrbit(true) {

    this->setName(name);
    sphere.mesh = Meshes.get("sphere");

    if (drawOrbit) {
        orbit = new Orbit();
        orbit->addTo(this);
    }

    scale = vec3f(radius);
    position = vec3f(orbRadius, 0, 0);
}

Planet::~Planet(){
}

void Planet::update(float deltaTime) {
    (void) deltaTime;

    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

    float dist = glm::length(cam->getPosition() - getPosition());
    float s = getScale().x;
    /*if (dist > 15.0*s) {
        sphere.mesh = Meshes.get("spheresuperlow");
    }
    else if (dist > 10.0*s) { // If we are far away we use the low poly sphere
        sphere.mesh = Meshes.get("spherelow");
    }
    else if (dist > 4.0*s) {
        sphere.mesh = Meshes.get("sphere");
    }*/
    //else 
    sphere.mesh = Meshes.get("spherehigh");
}

const GameObject* Planet::getArroundParent() const {
    return getParent();
}
