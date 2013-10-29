#include <glm/gtc/matrix_inverse.hpp>
#include "RingPlanet.hpp"
#include "Camera.hpp"

RingPlanet::RingPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram,
                       const std::string& texture, const std::string& ringtexture, const std::string& ringalphatexture, float distRingStart, float distRingEnd)
    : StandardPlanet(name, radius, orbRadius, shaderprogram, texture), ringtexture(ringtexture), ringalphatexture(ringalphatexture), distRingStart(radius+distRingStart), distRingEnd(radius+distRingEnd) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get(shaderprogram);
    ring.mesh = Meshes.get("square");
    ring.program = Programs.get("ringprogram");
}
RingPlanet::RingPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& bumpmap,
                       const std::string& texture, const std::string& ringtexture, const std::string& ringalphatexture,  float distRingStart, float distRingEnd)
     : StandardPlanet(name, radius, orbRadius, shaderprogram, texture, bumpmap), ringtexture(ringtexture), ringalphatexture(ringalphatexture), distRingStart(radius+distRingStart), distRingEnd(radius+distRingEnd) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get(shaderprogram);
    ring.mesh = Meshes.get("square");
    ring.program = Programs.get("ringprogram");
}

RingPlanet::~RingPlanet(){

}

void RingPlanet::draw() const {
    StandardPlanet::draw();

    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
    mat4f viewProjection = cam->projection*cam->view;
    mat4f ringTransform = glm::scale(glm::rotate(fullTransform, time, glm::normalize(vec3f(0.5,0.5,0))), vec3f(distRingEnd));

    ring.program->uniform("modelViewProjectionMatrix")->set(viewProjection*ringTransform);
    ring.program->uniform("modelMatrix")->set(ringTransform);
    ring.program->uniform("minCoord")->set(distRingStart/distRingEnd);

    Texture* ringTexture = Textures.get(ringtexture);
    ringTexture->bind();
    ring.program->uniform("texture")->set((int)ringTexture->getSlot());
    Texture* ringAlphaTexture = Textures.get(ringalphatexture);
    ringAlphaTexture->bind();
    ring.program->uniform("alphaTexture")->set((int)ringAlphaTexture->getSlot());

    glDisable(GL_CULL_FACE);
    ring.draw();
    glEnable(GL_CULL_FACE);

}
