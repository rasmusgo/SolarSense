#include "Earth.hpp"
#include "Planet.hpp"
#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"

Earth::Earth(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed)
                                : Planet(parentScene, parentObject->pos, scale)
                                , orbRadius(orbRadius), orbSpeed(orbSpeed) {
    sph.mesh = MeshManager::get("sphere");
    sph.program = ShaderManager::get("sample");

    orbit.mesh = MeshManager::get("square");
    orbit.program = ShaderManager::get("orbit");
}

Earth::~Earth() {
}

void Earth::update(float deltaTime) {
    (void) deltaTime;
    updateMatrix();
}

void Earth::draw() const {
    drawFrom(mat4f(1.0f));
}

void Earth::drawFrom(mat4f from) const {
    mat4f viewProjection = parentScene->getState().projection*parentScene->getState().view;
    mat4f transform = viewProjection*from*sph.modelMatrix;
    TextureManager::get("sun")->bind();

    sph.program->uniform("position")->set(2);
    sph.program->uniform("modelViewProjectionMatrix")->set(transform);
    sph.draw();
}
