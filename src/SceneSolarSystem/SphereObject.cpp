#include "SphereObject.hpp"
#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"

SphereObject::SphereObject(Scene* parentScene, vec3f pos, vec3f scale) : GameObject(parentScene,pos,scale) {
    sph.mesh = MeshManager::get("sphere");
    sph.program = ShaderManager::get("sample");

    mat4f m(1.0);
    m = glm::translate(m,pos);
    m = glm::scale(m,scale);
    sph.modelMatrix = m;
}

SphereObject::~SphereObject() {
}

void SphereObject::update(float deltaTime) {
    (void) deltaTime;
}

void SphereObject::updateMatrix() {

}

void SphereObject::draw() const {
    mat4f transform = parentScene->getState().projection*parentScene->getState().view*sph.modelMatrix;
    TextureManager::get("stars4K")->bind();
    sph.program->uniform("sampler")->set(2);
    sph.program->uniform("modelViewProjectionMatrix")->set(transform);
    sph.draw();
}

