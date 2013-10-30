#include "Orbit.hpp"
#include "Camera.hpp"
#include "Planet.hpp"

Orbit::Orbit() {
    orbit.mesh = Meshes.get("square");
    orbit.program = Programs.get("orbit");

    this->setDrawPriority(101);
}

Orbit::~Orbit() {
}

void Orbit::update(float deltaTime) {
    (void) deltaTime;
}

void Orbit::draw() const {
    const Planet* p = (Planet*)getParent();
        if (!p) return;
    const GameObject* pp = p->getArroundParent();
        if (!pp) return;

    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
    mat4f viewProjection = cam->projection*cam->view;

    float rad = p->orbRadius + p->radius;
    float orb = p->orbRadius/rad;
    //float distToCamera = glm::length(getPosition() - cam->getPosition());
    float width = p->radius/5.0f/rad;

    const WorldObject* wpp = dynamic_cast<const WorldObject*>(pp);
    const WorldObject* wp = dynamic_cast<const WorldObject*>(p);
    if (wpp && wp) {
        mat4f aux = glm::translate(mat4f(1.0f), wpp->getPosition());
        aux = aux * glm::mat4_cast(wpp->getRotation());
        mat4f orbTransform = glm::scale(aux, vec3f(rad));

        orbit.program->uniform("width")->set(width);
        orbit.program->uniform("orbit")->set(orb);
        orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
        orbit.program->uniform("modelMatrix")->set(orbTransform);
        orbit.program->uniform("modelViewProjectionMatrix")->set(viewProjection*orbTransform);
        orbit.program->uniform("planetPos")->set(wp->getPosition());
        orbit.program->uniform("parentPos")->set(wpp->getPosition());
        orbit.program->uniform("radius")->set(p->orbRadius);

        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        orbit.draw();
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
    }
}
