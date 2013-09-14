#include "OrbitingObject.hpp"

OrbitingObject::OrbitingObject(Scene* parentScene, GameObject* parentObject,
                               vec3f scale, float orbRadius, float orbSpeed)
                                : GameObject(parentScene, parentObject->pos, scale),
                                  orbitCenterObject(parentObject) {

}

OrbitingObject::~OrbitingObject() {
}

void OrbitingObject::update(float deltaTime) {

}

void OrbitingObject::updateMatrix() {

}

void OrbitingObject::draw() {

}

void OrbitingObject::addOrbitingObject(OrbitingObject *object) {
    addObject(object);

}
