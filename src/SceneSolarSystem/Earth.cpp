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
}

void Earth::draw() const {
    drawFrom(mat4f(1.0f));
}

void Earth::drawFrom(mat4f from) const {
    mat4f viewProjection = parentScene->getState().projection*parentScene->getState().view;
    mat4f transform = viewProjection*from*sphere.modelMatrix;
    TextureManager::get("earth")->bind();
    sphere.program->uniform("sampler")->set(2);
    sphere.program->uniform("a_position")->set(pos);
    sphere.program->uniform("a_normal")->set(vec3f(1,1,1));

    sphere.program->uniform("modelViewProjectionMatrix")->set(transform);
    sphere.draw();
}
