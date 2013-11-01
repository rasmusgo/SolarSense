#include <glm/gtc/matrix_inverse.hpp>
#include "RingPlanet.hpp"
#include "Camera.hpp"

RingPlanet::RingPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram,
                       const std::string& texture, const std::string& ringtexture, const std::string& ringalphatexture, float distRingStart, float distRingEnd, quat ringRotation)
    : StandardPlanet(name, radius, orbRadius, shaderprogram, texture), ringtexture(ringtexture), ringalphatexture(ringalphatexture), distRingStart(radius+distRingStart), distRingEnd(radius+distRingEnd),
      ringRotation(ringRotation) {
    sphere.mesh = Meshes.get("sphere");
    sphere.program = Programs.get(shaderprogram);
    ring.mesh = Meshes.get("square");
    ring.program = Programs.get("ringprogram");
}
RingPlanet::RingPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& bumpmap,
                       const std::string& texture, const std::string& ringtexture, const std::string& ringalphatexture,  float distRingStart, float distRingEnd, quat ringRotation)
     : StandardPlanet(name, radius, orbRadius, shaderprogram, texture, bumpmap), ringtexture(ringtexture), ringalphatexture(ringalphatexture), distRingStart(radius+distRingStart),
       distRingEnd(radius+distRingEnd), ringRotation(ringRotation) {
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
    quat invRotation = glm::inverse(getRotation());
    mat4f ringTransform = glm::scale(fullTransform * glm::mat4_cast(ringRotation * invRotation), vec3f(distRingEnd));

    ring.program->uniform("modelViewProjectionMatrix")->set(viewProjection*ringTransform);
    ring.program->uniform("modelMatrix")->set(ringTransform);
    ring.program->uniform("minCoord")->set(distRingStart/distRingEnd);
    ring.program->uniform("orbRadius")->set(orbRadius);
    ring.program->uniform("radius")->set(radius);
    ring.program->uniform("planetCenter")->set(getPosition());

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
