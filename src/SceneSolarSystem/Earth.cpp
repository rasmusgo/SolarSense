#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"
#include "Planet.hpp"
#include "Earth.hpp"

Earth::Earth(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed)
                                : Planet(parentScene, parentObject, scale, orbRadius, orbSpeed) {
    sphere.mesh = MeshManager::get("sphere");
    sphere.program = ShaderManager::get("earthShader");
}

Earth::~Earth(){

}


void Earth::update(float deltaTime) {
    (void) deltaTime;
    mat4f m(1.0);
    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed,vec3f(0,1,0));
    m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));
    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed*2,vec3f(0,1,0));
    m = glm::scale(m,scale);
    sphere.modelMatrix = m;
}

void Earth::draw() const {
    drawFrom(mat4f(1.0f));
}

void Earth::drawFrom(mat4f from) const {
    mat4f projection = parentScene->getState().projection;
    mat4f view = parentScene->getState().view;
    mat4f model = from*sphere.modelMatrix;
    mat4f transform = projection*view*model;

    vec3f lightPos = vec3f(0.0f);
    float shininess = 15.0f;
    vec3f emission = vec3f(0.1f);
    vec3f specular = vec3f(1.0f);
    vec3f lightAmbient = vec3f(1.0f);
    vec3f lightDiffuse(1.0f);
    vec3f lightSpecular(1.0f);

    sphere.program->uniform("lightPos")->set(lightPos);
    sphere.program->uniform("shininess")->set(shininess);
    sphere.program->uniform("emission")->set(emission);
    sphere.program->uniform("specular")->set(specular);
    sphere.program->uniform("lightAmbient")->set(lightAmbient);
    sphere.program->uniform("lightDiffuse")->set(lightDiffuse);
    sphere.program->uniform("lightSpecular")->set(lightSpecular);

    TextureManager::get("earth")->bind();
    sphere.program->uniform("sampler")->set(2);
    sphere.program->uniform("modelViewProjectionMatrix")->set(transform);
    sphere.program->uniform("modelMatrix")->set(model);
    sphere.program->uniform("viewMatrix")->set(view);
    sphere.draw();
}
