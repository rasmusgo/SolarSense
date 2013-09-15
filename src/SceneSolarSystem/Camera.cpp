#include "Camera.hpp"
#include "../input/InputManager.hpp"

Camera::Camera(Scene* scene, const vec3f &pos) : Entity(scene, pos, vec3f(1.0,1.0,1.0)),
    rot(0.0f,0.0f), rotM(1.0f), vel(1,1,1.5) {

}

Camera::~Camera() {
}

void Camera::draw() {

}

void Camera::update(float deltaTime) {
    //temporal mouseTest (arround a planet)
    if (InputManager::isMouseDown(sf::Mouse::Left)) {
        vec2i mouseDis = InputManager::getMouseDisplacement();
        vec2f newRot = vec2f(mouseDis.y, mouseDis.x)*0.2f;

        mat4f m(1.0);
        m = glm::rotate(m,newRot.y,vec3f(0,1,0));
        m = glm::rotate(m,newRot.x,vec3f(1,0,0));
        rotM = m*rotM;
    }

    if(InputManager::isKeyDown(sf::Keyboard::W)){
        pos += vec3f(0.0,0.0,-1.0)*vel*deltaTime;
    }
    if(InputManager::isKeyDown(sf::Keyboard::S)){
        pos += vec3f(0.0,0.0,1.0)*vel*deltaTime;
    }
    if(InputManager::isKeyDown(sf::Keyboard::D)){
        pos += vec3f(1.0,0.0,0.0)*vel*deltaTime;
    }
    if(InputManager::isKeyDown(sf::Keyboard::A)){
        pos += vec3f(-1.0,0.0,0.0)*vel*deltaTime;
    }
}

void Camera::drawHUD() {

}

mat4f Camera::getViewMatrix() {
    return (glm::translate(mat4f(1.0), -pos))*rotM;
}
