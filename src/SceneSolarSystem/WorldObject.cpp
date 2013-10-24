#include "WorldObject.hpp"

WorldObject::WorldObject() : position(0.0f) {

}

WorldObject::~WorldObject() {
}

void WorldObject::update(float deltaTime) {
    (void) deltaTime;

    transform = glm::translate(mat4f(1.0f), position);
    transform = transform * glm::mat4_cast(rotation);
}

void WorldObject::draw() const {
}

const vec3f WorldObject::getPosition() const {
    const GameObject* p = getParent();
    vec3f aux = position;

    if (p) {
        const WorldObject* wp = dynamic_cast<const WorldObject*>(p);
        if (wp) {
            const vec3f pp = wp->getPosition();

            aux = pp + aux;
        }
    }

    return aux;
}

const vec3f WorldObject::getLocalPosition() const {
    return position;
}

const quat WorldObject::getRotation() const {
    const GameObject* p = getParent();
    quat aux = rotation;

    if (p) {
        const WorldObject* wp = dynamic_cast<const WorldObject*>(p);
        if (wp) {
            const quat pp = wp->getRotation();

            aux = pp * aux;
        }
    }

    return aux;
}

const quat WorldObject::getLocalRotation() const {
    return rotation;
}
