#include "SphereObject.hpp"
#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"

SphereObject::SphereObject(Scene* parentScene, vec3f pos, vec3f scale) : GameObject(parentScene,pos,scale) {
    sph.mesh = MeshManager::get("sphere");
    sph.program = ShaderManager::get("sample");
}

SphereObject::~SphereObject() {
}

void SphereObject::update(float deltaTime) {
    (void) deltaTime;
    updateMatrix();
}

void SphereObject::updateMatrix() {
    mat4f m(1.0);
    m = glm::translate(m,pos);
    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*10,vec3f(0,1,0));
    m = glm::scale(m,scale);
    sph.modelMatrix = m;
}

void SphereObject::draw() const {
    mat4f transform = parentScene->getState().projection*parentScene->getState().view*sph.modelMatrix;
    TextureManager::get("sun")->bind();
    sph.program->uniform("sampler")->set(2);
    sph.program->uniform("modelViewProjectionMatrix")->set(transform);
    sph.draw();
}

