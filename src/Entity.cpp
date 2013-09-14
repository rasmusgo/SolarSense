#include "Entity.hpp"

Entity::Entity(Scene* scene, const vec3f &pos, const vec3f &scale) : GameObject(scene,pos,scale),
    vel(0,0,0), acc(0,0,0){
}

Entity::~Entity() {
}

void Entity::draw() const {
    GameObject::draw();
}

void Entity::update(float deltaTime) {
    GameObject::update(deltaTime);
}
