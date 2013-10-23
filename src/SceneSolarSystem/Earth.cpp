#include <glm/gtc/matrix_inverse.hpp>
#include "Planet.hpp"
#include "Earth.hpp"
#include "Camera.hpp"

Earth::Earth(const std::string& name) : Planet(name), time(0.0f) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get("earthShader");
}

Earth::~Earth(){

}

void Earth::update(float deltaTime) {
    time += deltaTime;

    transform = glm::rotate(mat4f(1.0f),time*orbSpeed,vec3f(0,1,0));
    transform = glm::translate(transform,vec3f(orbRadius, 0.0f, 0.0f));
    transform = glm::rotate(transform,time*orbSpeed*2,vec3f(0,1,0));
}

void Earth::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

//    mat4f t = glm::scale(fullTransform, vec3f(radius));
//    mat4f modelViewProjectionMatrix = cam->projection * cam->view * t;
//    mat4f modelViewMatrix =  t;
//    mat4f normalMatrix( glm::transpose(glm::inverse(modelViewMatrix)));

//    vec3f lightPos = vec3f(0,0,0); // parentObject->pos;

//    Texture* earth_day = TextureManager::get("earth_daytime");
//    earth_day->bind();
//    sphere.program->uniform("sampler")->set(2);

//    Texture* earth_night = TextureManager::get("earth_nighttime");
//    earth_night->bind();
//    sphere.program->uniform("EarthNight")->set(3);

//    Texture* earth_cloud = TextureManager::get("earth_cloud");
//    earth_cloud->bind();
//    sphere.program->uniform("EarthCloudGloss")->set(4);

//     Texture* earth_specular_map = TextureManager::get("earth_specular");
//     earth_specular_map->bind();
//     sphere.program->uniform("EarthSpecularMap")->set(5);

//    sphere.program->uniform("LightPosition")->set(lightPos);
//    sphere.program->uniform("MVPMatrix")->set(modelViewProjectionMatrix);
//    sphere.program->uniform("MVMatrix")->set(modelViewMatrix);
//    sphere.program->uniform("NormalMatrix")->set(normalMatrix);

    mat4f projection = cam->projection;
    mat4f view = cam->view;
    mat4f model = glm::scale(fullTransform, vec3f(radius));
    mat4f t = projection*view*model;

    vec3f lightPos = vec3f(0.0f);
    float shininess = 30.0f;
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

    Texture* tex;
    tex = Textures.get("earth");
    tex->bind();
    sphere.program->uniform("sampler")->set((int)tex->getSlot());
    tex = Textures.get("earthNight");
    tex->bind();
    sphere.program->uniform("samplerNight")->set((int)tex->getSlot());
    tex = Textures.get("earthWater");
    tex->bind();
    sphere.program->uniform("samplerWater")->set((int)tex->getSlot());
    sphere.program->uniform("modelViewProjectionMatrix")->set(t);
    sphere.program->uniform("modelMatrix")->set(model);
    sphere.program->uniform("viewMatrix")->set(view);

    sphere.draw();

    Planet::draw();
}
