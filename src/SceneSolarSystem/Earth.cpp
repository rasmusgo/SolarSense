#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"
#include "Planet.hpp"
#include "Earth.hpp"

Earth::Earth(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed)
                                : Planet(parentScene, parentObject, scale, orbRadius, orbSpeed) {
    sphere.mesh = MeshManager::get("sphere");
    sphere.program = ShaderManager::get("earthtest");
}

Earth::~Earth(){

}


void Earth::update(float deltaTime) {
    (void) deltaTime;
    mat4f m(1.0);
    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed,vec3f(0,1,0));
    m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));
    m = glm::scale(m,scale);
    sphere.modelMatrix = m;
}

void Earth::draw() const {
    drawFrom(mat4f(1.0f));
}

void Earth::drawFrom(mat4f from) const {
    mat4f viewProjection = parentScene->getState().projection*parentScene->getState().view;
    mat4f transform = viewProjection*from*sphere.modelMatrix;
    TextureManager::get("earth")->bind();
    sphere.program->uniform("sampler")->set(2);
    sphere.program->uniform("modelViewProjectionMatrix")->set(transform);
    sphere.draw();
}
