#include "WorldObject.hpp"

WorldObject::WorldObject() : position(0.0f), scale(1.0f) {

}

WorldObject::~WorldObject() {
}

void WorldObject::update(float deltaTime, float time) {
    (void) deltaTime;
    (void) time;

    transform = glm::translate(mat4f(1.0f), position);
    transform = transform * glm::mat4_cast(rotation);
    //transform = glm::scale(transform, scale);

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
            const quat pr = wp->getRotation();

            aux = pp + pr * aux;
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

const vec3f WorldObject::getScale() const {
    const GameObject* p = getParent();
    vec3f aux = scale;

    if (p) {
        const WorldObject* wp = dynamic_cast<const WorldObject*>(p);
        if (wp) {
            const vec3f pp = wp->getScale();

            aux = pp * aux;
        }
    }

    return aux;
}

const vec3f WorldObject::getLocalScale() const {
    return scale;
}

void WorldObject::onObjectAdd(GameObject* object) {
    WorldObject* wo = dynamic_cast<WorldObject*>(object);

    if (wo) {
        wo->position -= getPosition();
        wo->rotation = getRotation() * glm::inverse(wo->rotation);
        wo->scale /= getScale();
    }
}
