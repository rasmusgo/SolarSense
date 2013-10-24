#include "Camera.hpp"
#include "../input/SensorManager.hpp"

Camera::Camera(const vec3f& pos, const mat4f& projection) :
    projection(projection) {

    this->setName("cam");
    this->setUpdatePriority(2);
    this->projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);

    position = pos;

    mode = Free;
    maxAcc = 5.0f;
    friction = 5.0f;
    maxVel = 10.0f;
    eyeDistance3D = 0.03f;

    cameraClock.restart();

    originalPos = pos;
    interpolating = false;

    hudHand.mesh = Meshes.get("square");
    hudHand.program = Programs.get("hand");

    wasTracking = false;
}

Camera::~Camera() {
}

void Camera::draw() {
}

void Camera::drawHUD() {
    if (not wasTracking)
        handTime = GLOBALCLOCK.getElapsedTime().asSeconds();

    if (!SensorManager::isTracking()) {

        hudHand.program->uniform("time")->set(glm::mod(GLOBALCLOCK.getElapsedTime().asSeconds(), 20.0f));
        hudHand.program->uniform("lastTime")->set(handTime);
        hudHand.program->uniform("ratio")->set(((float)SCRWIDTH)/((float)SCRHEIGHT));

        Texture* tex = Textures.get("hand");
        tex->bind();
        hudHand.program->uniform("sampler")->set((int)tex->getSlot());

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        hudHand.draw();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    wasTracking = SensorManager::isTracking();
}

void Camera::update(float deltaTime) {
    deltaTime = cameraClock.restart().asSeconds();

    updateAcceleration(deltaTime);

    if (not interpolating) {
        mat4f m(1.0);
        float displ;
        switch (mode) {
            case Arround:
//                displ = (vel*deltaTime + 0.5f*acc*deltaTime*deltaTime).z;
//                pos.z += displ;
//                if (pos.z < arrObject->scale.x*1.5f) { //Too close! to the object!
//                    pos.z = arrObject->scale.x*1.5f;
//                }
//                else if (pos.z > 250.0) { //Where the fuck are you going!?
//                    pos.z = 250.0;
//                }

//                m = glm::rotate(m,vel.y/2.0f,vec3f(0,1,0));
//                m = glm::rotate(m,vel.x/2.0f,vec3f(1,0,0));
//                rotM = m*rotM;

                break;

            case Free:
                displ = (vel*deltaTime + 0.5f*acc*deltaTime*deltaTime).z;
                position += (vec3f(0,0,displ) * rotation);

                rotation = glm::rotate(rotation,vel.y/2.0f,vec3f(0,1,0));
                rotation = glm::rotate(rotation,vel.x/2.0f,(vec3f(1,0,0) * rotation));
        }
    }
    else { //Interpolating
//        interpolatingTimer += deltaTime;

//        //mat4f m = arrObject->getModelMatrix();
//        //vec3f wantedPos = -posFromMatrix(m) - pos;

//        vec3f p = posFromMatrix(rotM);
//        mat4f rot = glm::translate(mat4f(1.0f), -p)*rotM;

//        mat4f m = arrObject->getModelMatrix();
//        mat4f trans = glm::translate(mat4f(1.0f), -posFromMatrix(m));
//        mat4f posMatrix = glm::translate(mat4f(1.0f), -pos)*rot;
//        mat4f finalM = posMatrix*trans;
//        vec3f wantedPos = posFromMatrix(finalM);

//        vec3f newPos;
//        if (interpolatingTimer < INTERPOLATION_TIME) {
//            newPos = lerp(fromPos, wantedPos, interpolatingTimer/INTERPOLATION_TIME);
//            rotM[3][0] = newPos.x; rotM[3][1] = newPos.y; rotM[3][2] = newPos.z;
//        }
//        else {
//            rotM = rot;
//            interpolating = false;
//            SensorManager::resetInitialHandPos();
//        }
    }


    // Update transform matrix
    transform = glm::translate(glm::mat4_cast(rotation), -position);

    view = transform;
}

void Camera::updateAcceleration(float deltaTime) {
    acc = vec3f(0.0f);

    if (not interpolating) { //If not interpolating matrices we can change them
        if(Input::isKeyDown(sf::Keyboard::W)){
            acc.z = -maxAcc;
        }
        else if(Input::isKeyDown(sf::Keyboard::S)){
            acc.z = maxAcc;
        }
        if(Input::isKeyDown(sf::Keyboard::D)){
            acc.y = maxAcc;
        }
        else if(Input::isKeyDown(sf::Keyboard::A)){
            acc.y = -maxAcc;
        }
        if(Input::isKeyDown(sf::Keyboard::Q)){
            acc.x = maxAcc;
        }
        else if(Input::isKeyDown(sf::Keyboard::E)){
            acc.x = -maxAcc;
        }

        // Check SensorManager
        if (SensorManager::isTracking() && SensorManager::significantMovement()) {
            float speedFactor = 1.f;

            vec3f handMovement = SensorManager::getHandMovement();
            if (mode == Free)
                handMovement.x = -handMovement.x;
            else {
//                float distToObj = (pos.z - arrObject->scale.x) / glm::min(arrObject->scale.x, 20.0f);
//                speedFactor = glm::min((distToObj - 0.5)/ 5.f, 0.8)  + 0.2f; // 0.5 is the minimum value for distToObject, 5 is the maximum value for distToObj
            }

            handMovement.z *= speedFactor;
            handMovement *= maxVel*speedFactor;

            vel = handMovement;
        }

        if (Input::isMouseDown(sf::Mouse::Left)) {
            vec2i dis = Input::getMouseDisplacement();
            vec2f mouseDis = vec2f(dis.x, dis.y);
            vec2f scr = vec2f(SCRWIDTH/100, SCRHEIGHT/100);
            vec2f mouseDisRel = vec2f(mouseDis.y/scr.y, mouseDis.x/scr.x);
            vec2f newRot = mouseDisRel*maxAcc;
            //std::cout << mouseDis.y << "," << mouseDisRel.y << "," << newRot.y << std::endl;

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
//    if (mode == Arround) {
//        mat4f m2(1.0f);
//        m2 = arrObject->getModelMatrix();
//        m2 = glm::translate(mat4f(1.0f), -posFromMatrix(m2));
//        rotM = glm::translate(mat4f(1.0f), -pos)*rotM*m2;
//    }

//    arrObject = object;
//    //rotM = mat4f(1.0f);
//    pos = vec3f(0,0,object->scale.x*3);
//    fromPos = posFromMatrix(rotM);
//    mode = Arround;

//    interpolating = true;
//    interpolatingTimer = 0.0f;
}

void Camera::setMode(CameraMode m) {
//    mat4f m2 (1.0f);
//    switch (m) {
//        case Free:
//            if (mode == Arround) {
//                m2 = arrObject->getModelMatrix();
//                m2 = glm::translate(mat4f(1.0f), -posFromMatrix(m2));
//                rotM = glm::translate(mat4f(1.0f), -pos)*rotM*m2;
//            }
//            break;
//        default:
//            break;
//    }

//    mode = m;
}

//mat4f Camera::getViewMatrix() {
//    switch (mode) {
//    case Free:
//        return rotM;
//    case Arround:
//        if (not interpolating) {
//            if (arrObject) {
//                mat4f m = arrObject->getModelMatrix();
//                mat4f trans = glm::translate(mat4f(1.0f), -posFromMatrix(m));
//                return glm::translate(mat4f(1.0f), -pos)*rotM*trans;
//            }
//            else return glm::translate(mat4f(1.0f), -pos)*rotM;
//        }
//        else {
//            return rotM;
//        }
//    default:
//        return rotM;
//    }
//}

std::pair<mat4f,mat4f> Camera::getViewMatrix3D() {
    mat4f c = view;
    mat4f r(1.0f), l(1.0f);
    r = glm::translate(r, vec3f(1,0,0)*(eyeDistance3D/2.0f));
    l = glm::translate(l, -vec3f(1,0,0)*(eyeDistance3D/2.0f));

    return std::pair<mat4f,mat4f>(r*c,l*c); //Right and Left
}

mat4f Camera::billboard (vec3f position) {
    vec3f cameraPos = -getPosition();
    vec3f cameraUp = getRotation() * vec3f(0,1,0);

    vec3f look = glm::normalize(cameraPos - position);
    vec3f right = glm::cross(cameraUp, look);
    vec3f up2 = glm::cross(look, right);
    mat4f transform;
    transform[0] = vec4f(right, 0);
    transform[1] = vec4f(up2, 0);
    transform[2] = vec4f(look, 0);
    // Uncomment this line to translate the position as well
    // (without it, it's just a rotation)
    //transform[3] = vec4f(position, 0);
    return transform;
}

