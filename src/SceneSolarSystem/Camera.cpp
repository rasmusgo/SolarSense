#include "Camera.hpp"
#include "../input/KeyAndMouseManager.hpp"
#include "../input/SensorManager.hpp"

Camera::Camera(Scene* scene, const vec3f &pos) : Entity(scene, pos, vec3f(1.0,1.0,1.0)),
    rot(0.0f,0.0f), rotM(1.0f) {

    mode = Free;
    maxAcc = 5.0f;
    friction = 5.0f;
    maxVel = 10.0f;

    rotM = glm::translate(mat4f(1.0), -pos);
    cameraClock.restart();
}

Camera::~Camera() {
}

void Camera::draw() {
}

void Camera::update(float deltaTime) {
    deltaTime = cameraClock.restart().asSeconds();

    acc = vec3f(0.0f);
    if(KeyAndMouseManager::isKeyDown(sf::Keyboard::W)){
        acc.z = -maxAcc;
    }
    else if(KeyAndMouseManager::isKeyDown(sf::Keyboard::S)){
        acc.z = maxAcc;
    }
    if(KeyAndMouseManager::isKeyDown(sf::Keyboard::D)){
        acc.x = maxAcc;
    }
    else if(KeyAndMouseManager::isKeyDown(sf::Keyboard::A)){
        acc.x = -maxAcc;
    }
    if(KeyAndMouseManager::isKeyDown(sf::Keyboard::Q)){
        acc.y = maxAcc;
    }
    else if(KeyAndMouseManager::isKeyDown(sf::Keyboard::E)){
        acc.y = -maxAcc;
    }

    /*// Check SensorManager
    if (SensorManager::isTracking() && SensorManager::significantMovement()) {
        float speedFactor = 0.02f;

        vec3f handMovement = SensorManager::getHandMovement();
        //acc = vec3f(handMovement.y*speedFactor, handMovement.x*speedFactor, handMovement.z*speedFactor*2);

        handMovement *= speedFactor;

        mat4f m(1.0);
        m = glm::rotate(m,handMovement.x,vec3f(1,0,0));
        m = glm::rotate(m,handMovement.y,vec3f(0,1,0));
        acc.z = handMovement.z * 4;
        rotM = m*rotM;
    }*/

    if (glm::length(acc) > 0.1f) {
        vel += acc*deltaTime;
    }
    if (glm::length(vel) > 0.1f) {
        if (glm::abs(acc.x) < 0.1) {
            if (vel.x > 0.1f) vel.x = glm::max(0.0f, vel.x - friction*deltaTime);
            else if (vel.x < -0.1f) vel.x = glm::min(0.0f, vel.x + friction*deltaTime);
            else vel.x = 0.0f;
        }
        if (glm::abs(acc.y) < 0.1) {
            if (vel.y > 0.1f) vel.y = glm::max(0.0f, vel.y - friction*deltaTime);
            else if (vel.y < -0.1f) vel.y = glm::min(0.0f, vel.y + friction*deltaTime);
            else vel.y = 0.0f;
        }
        if (glm::abs(acc.z) < 0.1) {
            if (vel.z > 0.1f) vel.z = glm::max(0.0f, vel.z - friction*deltaTime);
            else if (vel.z < -0.1f) vel.z = glm::min(0.0f, vel.z + friction*deltaTime);
            else vel.z = 0.0f;
        }

        if (glm::length(vel) > maxVel) vel = glm::normalize(vel)*maxVel;
    }

    switch (mode) {
        case Arround:
            if (KeyAndMouseManager::isMouseDown(sf::Mouse::Left)) {
                vec2i mouseDis = KeyAndMouseManager::getMouseDisplacement();
                vec2f newRot = vec2f(mouseDis.y, mouseDis.x)*0.2f;

                mat4f m(1.0);
                m = glm::rotate(m,newRot.y,vec3f(0,1,0));
                m = glm::rotate(m,newRot.x,vec3f(1,0,0));
                rotM = m*rotM;
            }

            //pos += vel*deltaTime + 0.5f*acc*deltaTime*deltaTime;
            break;

        case Free:

            vec3f displ = vel*deltaTime + 0.5f*acc*deltaTime*deltaTime;
            pos += displ;
            mat4f t(1.0);
            t = glm::translate(t, -displ);
            rotM = t*rotM;

            if (KeyAndMouseManager::isMouseDown(sf::Mouse::Left)) {
                vec2i mouseDis = KeyAndMouseManager::getMouseDisplacement();
                vec2f mouseDisRel = vec2f(((float)mouseDis.y)/SCRHEIGHT, ((float)mouseDis.x)/SCRWIDTH);
                vec2f newRot = mouseDisRel*100.0f;

                mat4f m(1.0);
                m = glm::rotate(m,newRot.y,vec3f(0,1,0));
                m = glm::rotate(m,newRot.x,vec3f(1,0,0));
                rotM = m*rotM;
            }
    }
}

void Camera::setArround(GameObject *object) {
    arrObject = object;
}

void Camera::drawHUD() {

}

mat4f Camera::getViewMatrix() {
    return rotM;
}
