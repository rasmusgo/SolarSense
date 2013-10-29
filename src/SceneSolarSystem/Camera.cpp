#include "Camera.hpp"
#include "../input/SensorManager.hpp"

Camera::Camera(const vec3f& pos, const mat4f& projection) :
    projection(projection), stereoscopic3D(false) {

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
    focus = 375.0f;

    wasTracking = false;
}

Camera::~Camera() {
}

void Camera::draw() {
}

void Camera::drawHUD() {
    if (not wasTracking)
        handTime = GLOBALCLOCK.getElapsedTime().asSeconds();

    if (SensorManager::sensorConnected() && !SensorManager::isTracking()) {

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
        float displ, objScale;
        switch (mode) {
            case Arround:
                rotation = glm::rotate(rotation,vel.y/2.0f,vec3f(0,1,0));
                rotation = glm::rotate(rotation,vel.x/2.0f,(vec3f(1,0,0) * rotation));

                objScale = arrObject->getScale().x;

                displ = (vel*deltaTime + 0.5f*acc*deltaTime*deltaTime).z;
                lastArrDist += displ;
                if (lastArrDist < objScale*1.5f) { //Too close! to the object!
                    lastArrDist = objScale*1.5f;
                }
                else if (lastArrDist > 250.0) { //Where the fuck are you going!?
                    lastArrDist = 250.0;
                }
                position = arrObject->getPosition() + (vec3f(0,0,lastArrDist) * rotation);
                //lastArrDist = glm::length(position - arrObject->getPosition());
                break;

            case Free:
                displ = (vel*deltaTime + 0.5f*acc*deltaTime*deltaTime).z;
                position += (vec3f(0,0,displ) * rotation);

                rotation = glm::rotate(rotation,vel.y/2.0f,vec3f(0,1,0));
                rotation = glm::rotate(rotation,vel.x/2.0f,(vec3f(1,0,0) * rotation));
        }
    }
    else { //Interpolating
        rotation = glm::rotate(rotation,vel.y/2.0f,vec3f(0,1,0));
        rotation = glm::rotate(rotation,vel.x/2.0f,(vec3f(1,0,0) * rotation));

        interpolatingTimer += deltaTime;

        vec3f wantedPos = arrObject->getPosition() + vec3f(0,0, arrObject->getScale().x*3.0f) * rotation;

        if (interpolatingTimer < INTERPOLATION_TIME) {
            position = Utils::lerp(fromPos, wantedPos, interpolatingTimer/INTERPOLATION_TIME);
        }
        else {
            interpolating = false;
            SensorManager::resetInitialHandPos();
            lastArrDist = glm::length(position - arrObject->getPosition());
        }
    }


    // Update transform matrix
    transform = glm::translate(glm::mat4_cast(rotation), -position);

    view = transform;

    if (stereoscopic3D) {
        eyes = getViewMatrix3D();

        focusSpeed = 0.01f*deltaTime;

        setEye(0);
        currEye = 0;
    }
}

void Camera::updateAcceleration(float deltaTime) {
    acc = vec3f(0.0f);

    //if (not interpolating) { //If not interpolating matrices we can change them
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
                float distToObj = glm::length(position - arrObject->getPosition()) / glm::min(arrObject->getScale().x, 20.0f);
                speedFactor = glm::min((distToObj - 0.5)/ 5.f, 0.8)  + 0.2f; // 0.5 is the minimum value for distToObject, 5 is the maximum value for distToObj
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
    //}

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

void Camera::setArround(const std::string& objectName) {
    WorldObject* obj = dynamic_cast<WorldObject*>(getGame()->getObjectByName(objectName));

    if (obj)
        setArround(obj);
}

void Camera::setArround(WorldObject* obj) {
    arrObject = obj;
    mode = Arround;

    fromPos = position;

    interpolating = true;
    interpolatingTimer = 0.0f;
}

void Camera::setMode(CameraMode m) {
    mode = m;
}

std::pair<mat4f,mat4f> Camera::getViewMatrix3D() {
    mat4f c = view;
    mat4f r(1.0f), l(1.0f);
    r = glm::translate(r, vec3f(1,0,0)*(eyeDistance3D/2.0f));
    l = glm::translate(l, -vec3f(1,0,0)*(eyeDistance3D/2.0f));

    return std::pair<mat4f,mat4f>(r*c,l*c); //Right and Left
}

mat4f Camera::billboard (vec3f position) {
    vec3f cameraPos = getPosition();
    vec3f billboardLookVector = glm::normalize(cameraPos - position);
    vec3f cameraUp = vec3f(0,1,0)* getRotation();

    vec3f right = glm::cross(cameraUp, billboardLookVector);
    vec3f up = glm::cross(right,billboardLookVector);


    mat4f transform;
    //d = position - ((right - up) * scale);
    transform[0] = vec4f(right, 0);
    transform[1] = vec4f(up, 0);
    transform[2] = vec4f(billboardLookVector, 0);
    // Uncomment this line to translate the position as well
    // (without it, it's just a rotation)
    //transform[3] = vec4f(position, 0);
    return transform;
}

void Camera::setStereoscopic(bool s3d) {
    if (s3d) {
        stereoscopic3D = s3d;
    }
    else {
        glViewport(0,0,float(SCRWIDTH),float(SCRHEIGHT)); //back to normal

        this->projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);
    }
}

void Camera::setEye(int i) {
    float ratio  = float(SCRWIDTH) / float(SCRHEIGHT);
    float radians = DEG_TO_RAD * FOV / 2.0;
    float wd2     = ZNEAR * glm::tan(radians);
    //float ndfl    = ZNEAR / ((ZFAR - ZNEAR)*0.5);
    if (mode == Arround) {
        focus = Utils::lerp(focus, glm::length(getPosition() - arrObject->getPosition()), focusSpeed);
    }
    else focus = focus = Utils::lerp(focus, 375.0f, focusSpeed);
    float ndfl = ZNEAR / focus;

    float left, right, top, bottom;

    if (i == 0) {
        glViewport(float(SCRWIDTH)/2.0,0,float(SCRWIDTH)/2.0,float(SCRHEIGHT)); //Right eye
        {
            //Off Axis projection
            left  = - ratio * wd2 - 0.5 * eyeDistance3D * ndfl;
            right =   ratio * wd2 - 0.5 * eyeDistance3D * ndfl;
            top    =   wd2;
            bottom = - wd2;
            projection = glm::frustum(left,right,bottom,top,ZNEAR,ZFAR);

            view = eyes.first;
        }
    }
    else {
        glViewport(0,0,float(SCRWIDTH)/2.0,float(SCRHEIGHT)); //Left eye
        {
            //Off Axis projection
            left  = - ratio * wd2 + 0.5 * eyeDistance3D * ndfl;
            right =   ratio * wd2 + 0.5 * eyeDistance3D * ndfl;
            top    =   wd2;
            bottom = - wd2;
            projection = glm::frustum(left,right,bottom,top,ZNEAR,ZFAR);

            view = eyes.second;
        }
    }
}
