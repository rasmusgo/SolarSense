#include <glm/gtc/matrix_inverse.hpp>
#include "StandardPlanet.hpp"
#include "Camera.hpp"

StandardPlanet::StandardPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& texture)
                                : Planet(name, radius, orbRadius), shaderprogram(shaderprogram), texturemapname(texture) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get(shaderprogram);
    hasBump = false;
}
StandardPlanet::StandardPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& texture, const std::string& bumpmap)
                                : Planet(name, radius, orbRadius), bumpmap(bumpmap), shaderprogram(shaderprogram), texturemapname(texture) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get(shaderprogram);
    hasBump = true;
}

StandardPlanet::~StandardPlanet(){

}

void StandardPlanet::update(float deltaTime) {
    time += deltaTime;

    position = vec3f(orbRadius*cos(time*orbSpeed), 0.0f, orbRadius*sin(time*orbSpeed));
    rotation = glm::rotate(rotation, deltaTime*rotSpeed, vec3f(0,1,0));

    WorldObject::update(deltaTime);
    Planet::update(deltaTime);
}

void StandardPlanet::draw() const {
    if(hasBump) {drawBump(); return;}
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

    mat4f t = glm::scale(fullTransform, getScale());
    mat4f modelViewProjectionMatrix = cam->projection * cam->view * t;
    mat4f modelViewMatrix = t;
    mat4f normalMatrix( glm::transpose(glm::inverse(modelViewMatrix)));

    vec3f lightPos = vec3f(0,0,0); // The sun is in the 0,0,0

    Texture* sdp_tex = Textures.get(texturemapname);
    sdp_tex->bind();

    sphere.program->uniform("sampler")->set((int)sdp_tex->getSlot());
    sphere.program->uniform("LightPosition")->set(lightPos);
    sphere.program->uniform("MVPMatrix")->set(modelViewProjectionMatrix);
    sphere.program->uniform("MVMatrix")->set(modelViewMatrix);
    sphere.program->uniform("NormalMatrix")->set(normalMatrix);

    sphere.draw();
}
void StandardPlanet::drawBump() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
    mat4f projection = cam->projection;
    mat4f view = cam->view;
    mat4f model = glm::scale(fullTransform, getScale());
    mat4f t = projection*view*model;
    mat4f normalMatrix( glm::transpose(glm::inverse(model)));

    vec3f lightPos = vec3f(0.0f);// - position;
    float shininess = 20.0f;
    vec3f emission = vec3f(0.0f);
    vec3f specular = vec3f(0.5f);
    vec3f lightAmbient = vec3f(0.0f);
    vec3f lightDiffuse(1.0f);
    vec3f lightSpecular(1.0f);

    sphere.program->uniform("lightPos")->set(lightPos);
    sphere.program->uniform("shininess")->set(shininess);
    sphere.program->uniform("emission")->set(emission);
    sphere.program->uniform("specular")->set(specular);
    sphere.program->uniform("lightAmbient")->set(lightAmbient);
    sphere.program->uniform("lightDiffuse")->set(lightDiffuse);
    //sphere.program->uniform("lightSpecular")->set(lightSpecular);

    Texture* tex;
    tex = Textures.get(texturemapname);
    tex->bind();
    sphere.program->uniform("sampler")->set((int)tex->getSlot());

    tex = Textures.get(bumpmap);
    tex->bind();
    sphere.program->uniform("samplerNormal")->set((int)tex->getSlot());
    
    sphere.program->uniform("modelViewProjectionMatrix")->set(t);
    sphere.program->uniform("modelMatrix")->set(model);
    sphere.program->uniform("viewMatrix")->set(view);
    sphere.program->uniform("normalMatrix")->set(normalMatrix);

    sphere.draw();

}