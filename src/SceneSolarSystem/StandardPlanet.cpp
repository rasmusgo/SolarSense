#include <glm/gtc/matrix_inverse.hpp>
#include "StandardPlanet.hpp"
#include "Camera.hpp"

StandardPlanet::StandardPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& texture)
                                : Planet(name, radius, orbRadius), shaderprogram(shaderprogram), texturemapname(texture) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get(shaderprogram);
}
StandardPlanet::StandardPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& texture, const std::string& bumpmap)
                                : Planet(name, radius, orbRadius), bumpmap(bumpmap), shaderprogram(shaderprogram), texturemapname(texture) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get(shaderprogram);
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

    Planet::draw();
}
