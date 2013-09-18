#include "SceneSolarSystem.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include "../SolarSenseApp.hpp"
#include "Planet.hpp"
#include "Earth.hpp"

Earth::Earth(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed)
                                : Planet(parentScene, parentObject, scale, orbRadius, orbSpeed), parentObject(parentObject) {
    sphere.mesh = MeshManager::get("sphere");
    sphere.program = ShaderManager::get("earthtest");
}

Earth::~Earth(){

}


void Earth::update(float deltaTime) {
    (void) deltaTime;
    mat4f m(1.0);
    m = glm::rotate(m,0.0f/*GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed*/,vec3f(0,1,0));
    m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));
    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed*2,vec3f(0,1,0));
    m = glm::scale(m,scale);
    sphere.modelMatrix = m;
}

void Earth::draw() const {
    drawFrom(mat4f(1.0f));
}

void Earth::drawFrom(mat4f from) const {
    mat4f modelViewProjectionMatrix = parentScene->getState().projection * parentScene->getState().view * sphere.modelMatrix;
    mat4f modelViewMatrix =  sphere.modelMatrix;
    mat4f normalMatrix( glm::transpose(glm::inverse(modelViewMatrix)));

    vec3f LightPosition = parentObject->pos;

    Texture* earth_night = TextureManager::get("earth_nighttime");
    earth_night->bind();
    sphere.program->uniform("EarthNight")->set((int)earth_night->getSlot());   

    Texture* earth_day = TextureManager::get("earth_daytime");
    earth_day->bind();
    sphere.program->uniform("EarthDay")->set((int)earth_day->getSlot());   



    sphere.program->uniform("LightPosition")->set(LightPosition);
    sphere.program->uniform("MVPMatrix")->set(modelViewProjectionMatrix);
    sphere.program->uniform("MVMatrix")->set(modelViewMatrix);
    sphere.program->uniform("NormalMatrix")->set(normalMatrix);

    sphere.draw();
}
