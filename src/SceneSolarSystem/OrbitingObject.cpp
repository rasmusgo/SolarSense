#include "OrbitingObject.hpp"
#include "Planet.hpp"
#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"

OrbitingObject::OrbitingObject(Scene* parentScene, GameObject* parentObject,
                               vec3f scale, float orbRadius, float orbSpeed)
                                : GameObject(parentScene, parentObject->pos, scale)
                                , orbRadius(orbRadius), orbSpeed(orbSpeed) {
    sph.mesh = MeshManager::get("sphere");
    sph.program = ShaderManager::get("sample");

    orbit.mesh = MeshManager::get("square");
    orbit.program = ShaderManager::get("orbit");
}

OrbitingObject::~OrbitingObject() {
}

void OrbitingObject::update(float deltaTime) {
    (void) deltaTime;
    updateMatrix();
}

void OrbitingObject::updateMatrix() {
    mat4f m(1.0);
    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed,vec3f(0,1,0));
    m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));
    baseMatrix = m;
    m = glm::scale(m,scale);
    sph.modelMatrix = m;
}

void OrbitingObject::draw() const {
    drawFrom(mat4f(1.0f));
}

void OrbitingObject::addOrbitingObject(OrbitingObject* orb) {
    orbObjects.push_back(orb);
}

void OrbitingObject::drawFrom(mat4f from) const {
    mat4f viewProjection = parentScene->getState().projection*parentScene->getState().view;
    mat4f transform = viewProjection*from*sph.modelMatrix;
    TextureManager::get("sun")->bind();
    sph.program->uniform("sampler")->set(2);
    sph.program->uniform("modelViewProjectionMatrix")->set(transform);
    sph.draw();

    //Draw sons
    for(std::list<OrbitingObject*>::const_iterator it = orbObjects.begin(); it != orbObjects.end(); ++it) {
        //Draw orbit of son
        float rad = (*it)->orbRadius + (*it)->scale.x;
        float orb = (*it)->orbRadius/rad;
        mat4f orbTransform = glm::scale(baseMatrix, vec3f(rad, rad, rad));

        orbit.program->uniform("width")->set(0.005f);
        orbit.program->uniform("orbit")->set(orb);
        orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
        orbit.program->uniform("modelViewProjectionMatrix")->set(viewProjection*orbTransform);
        glDisable(GL_CULL_FACE);
        orbit.draw();
        glEnable(GL_CULL_FACE);

        (*it)->drawFrom(baseMatrix);
    }
}
