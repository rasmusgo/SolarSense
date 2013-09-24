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

    originalPos = pos;
    interpolating = false;
}

Camera::~Camera() {
}

void Camera::draw() {
}

void Camera::update(float deltaTime) {
    deltaTime = cameraClock.restart().asSeconds();

    updateAcceleration(deltaTime);

    if (not interpolating) {
        mat4f m(1.0);
        float displ;
        switch (mode) {
            case Arround:
                displ = (vel*deltaTime + 0.5f*acc*deltaTime*deltaTime).z;
                pos.z += displ;

                m = glm::rotate(m,vel.y/2.0f,vec3f(0,1,0));
                m = glm::rotate(m,vel.x/2.0f,vec3f(1,0,0));
                rotM = m*rotM;

                break;

            case Free:
                displ = (vel*deltaTime + 0.5f*acc*deltaTime*deltaTime).z;
                pos.z += displ;
                m = glm::translate(m, -vec3f(0,0,displ));
                rotM = m*rotM;

                m = mat4f(1.0);
                m = glm::rotate(m,vel.y/2.0f,vec3f(0,1,0));
                m = glm::rotate(m,vel.x/2.0f,vec3f(1,0,0));
                rotM = m*rotM;
        }
    }
    else { //Interpolating
        interpolatingTimer += deltaTime;

        //mat4f m = arrObject->getModelMatrix();
        //vec3f wantedPos = -posFromMatrix(m) - pos;

        vec3f p = posFromMatrix(rotM);
        mat4f rot = glm::translate(mat4f(1.0f), -p)*rotM;

        mat4f m = arrObject->getModelMatrix();
        mat4f trans = glm::translate(mat4f(1.0f), -posFromMatrix(m));
        mat4f posMatrix = glm::translate(mat4f(1.0f), -pos)*rot;
        mat4f finalM = posMatrix*trans;
        vec3f wantedPos = posFromMatrix(finalM);

        vec3f newPos;
        if (interpolatingTimer < INTERPOLATION_TIME) {
            newPos = lerp(fromPos, wantedPos, interpolatingTimer/INTERPOLATION_TIME);
            rotM[3][0] = newPos.x; rotM[3][1] = newPos.y; rotM[3][2] = newPos.z;
        }
        else {
            rotM = rot;
            interpolating = false;
        }


    }
}

void Camera::updateAcceleration(float deltaTime) {
    acc = vec3f(0.0f);

    if (not interpolating) { //If not interpolating matrices we can change them
        if(KeyAndMouseManager::isKeyDown(sf::Keyboard::W)){
            acc.z = -maxAcc;
        }
        else if(KeyAndMouseManager::isKeyDown(sf::Keyboard::S)){
            acc.z = maxAcc;
        }
        if(KeyAndMouseManager::isKeyDown(sf::Keyboard::D)){
            acc.y = maxAcc;
        }
        else if(KeyAndMouseManager::isKeyDown(sf::Keyboard::A)){
            acc.y = -maxAcc;
        }
        if(KeyAndMouseManager::isKeyDown(sf::Keyboard::Q)){
            acc.x = maxAcc;
        }
        else if(KeyAndMouseManager::isKeyDown(sf::Keyboard::E)){
            acc.x = -maxAcc;
        }

        // Check SensorManager
        if (SensorManager::isTracking()) {

            vec3f handMovement = SensorManager::getHandMovement();

            vec3f aux = vec3f(abs(handMovement.x), abs(handMovement.y), abs(handMovement.z));
            float maxVal = (aux.x > aux.y)? ((aux.x > aux.z)? aux.x : aux.z) : ((aux.y > aux.z)? aux.y : aux.z);
            maxVal = glm::max(maxVal, (float)MOVEMENT_THRESHOLD);
            handMovement /= maxVal;
            handMovement *= maxAcc;

            acc = handMovement;
        }

        if (KeyAndMouseManager::isMouseDown(sf::Mouse::Left)) {
            vec2i dis = KeyAndMouseManager::getMouseDisplacement();
            vec2f mouseDis = vec2f(dis.x, dis.y);
            vec2f scr = vec2f(SCRWIDTH/100, SCRHEIGHT/100);
            vec2f mouseDisRel = vec2f(mouseDis.y/scr.y, mouseDis.x/scr.x);
            vec2f newRot = mouseDisRel*maxAcc;
            std::cout << mouseDis.y << "," << mouseDisRel.y << "," << newRot.y << std::endl;

            if (newRot.x > 0.1f) acc.x = glm::min(maxAcc, newRot.x);
            else if (newRot.x < -0.1f) acc.x = glm::max(-maxAcc, newRot.x);
            if (newRot.y > 0.1f) acc.y = glm::min(maxAcc, newRot.y);
            else if (newRot.y < -0.1f) acc.y = glm::max(-maxAcc, newRot.y);
        }
    }

    //Friction and physics
    float minVal = 0.1f;
    if (glm::length(acc) > minVal) {
        vel += acc*deltaTime;
    }
    if (glm::length(vel) > minVal) {
        if (glm::abs(acc.x) < minVal) {
            if (vel.x > minVal) vel.x = glm::max(0.0f, vel.x - friction*deltaTime);
            else if (vel.x < -minVal) vel.x = glm::min(0.0f, vel.x + friction*deltaTime);
            else vel.x = 0.0f;
        }
        if (glm::abs(acc.y) < minVal) {
            if (vel.y > minVal) vel.y = glm::max(0.0f, vel.y - friction*deltaTime);
            else if (vel.y < -minVal) vel.y = glm::min(0.0f, vel.y + friction*deltaTime);
            else vel.y = 0.0f;
        }
        if (glm::abs(acc.z) < minVal) {
            if (vel.z > minVal) vel.z = glm::max(0.0f, vel.z - friction*deltaTime);
            else if (vel.z < -minVal) vel.z = glm::min(0.0f, vel.z + friction*deltaTime);
            else vel.z = 0.0f;
        }

        if (glm::length(vel) > maxVel) vel = glm::normalize(vel)*maxVel;
        else if (glm::length(vel) < minVal) vel = vec3f(0.0f);
    }
}

void Camera::setArround(GameObject *object) {
    if (mode == Arround) {
        mat4f m2(1.0f);
        m2 = arrObject->getModelMatrix();
        m2 = glm::translate(mat4f(1.0f), -posFromMatrix(m2));
        rotM = glm::translate(mat4f(1.0f), -pos)*rotM*m2;
    }

    arrObject = object;
    //rotM = mat4f(1.0f);
    pos = vec3f(0,0,object->scale.x*3);
    fromPos = posFromMatrix(rotM);
    mode = Arround;

    interpolating = true;
    interpolatingTimer = 0.0f;
}

void Camera::setMode(CameraMode m) {
    mat4f m2 (1.0f);
    switch (m) {
        case Free:
            if (mode == Arround) {
                m2 = arrObject->getModelMatrix();
                m2 = glm::translate(mat4f(1.0f), -posFromMatrix(m2));
                rotM = glm::translate(mat4f(1.0f), -pos)*rotM*m2;
            }
            break;
        default:
            break;
    }

    mode = m;
}

void Camera::drawHUD() {

}

mat4f Camera::getViewMatrix() {
    switch (mode) {
    case Free:
        return rotM;
    case Arround:
        if (not interpolating) {
            if (arrObject) {
                mat4f m = arrObject->getModelMatrix();
                mat4f trans = glm::translate(mat4f(1.0f), -posFromMatrix(m));
                return glm::translate(mat4f(1.0f), -pos)*rotM*trans;
            }
            else return glm::translate(mat4f(1.0f), -pos)*rotM;
        }
        else {
            return rotM;
        }
    default:
        return rotM;
    }
}

inline vec3f Camera::posFromMatrix(mat4f &m) {
    return vec3f(m[3][0],m[3][1],m[3][2]);
}

inline vec3f Camera::lerp(vec3f& from, vec3f& to, float t) {
    return to*t + from*(1.0f-t) ;
}
