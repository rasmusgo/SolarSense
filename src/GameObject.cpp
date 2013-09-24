#include "GameObject.hpp"
#include "Scene.hpp"

GameObject::GameObject(Scene* scene, const vec3f &pos, const vec3f &scale) :
	isAlive(true), pos(pos), scale(scale),
	parentScene(scene) {
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
	(void) deltaTime;
}

void GameObject::draw() const {
}

void GameObject::addObject(GameObject *object) {
	objects.push_back(object);
}

mat4f GameObject::getModelMatrix() {
    return glm::translate(mat4f(1.0f), pos);
}
