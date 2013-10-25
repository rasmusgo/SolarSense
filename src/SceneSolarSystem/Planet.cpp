#include "Planet.hpp"
#include "Camera.hpp"

Planet::Planet(const std::string& name, float radius, float orbRadius)
    : radius(radius), orbRadius(orbRadius), orbSpeed(0.0f), drawOrbit(true) {

    this->setName(name);
    orbit.mesh = Meshes.get("square");
    orbit.program = Programs.get("orbit");

    scale = vec3f(radius);
    position = vec3f(orbRadius, 0, 0);
}

Planet::~Planet(){
}

void Planet::draw() const {
    const GameObject* p = getParent();
    if (p and drawOrbit) {
        Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
        mat4f viewProjection = cam->projection*cam->view;

        float rad = orbRadius + radius;
        float orb = orbRadius/rad;

        const WorldObject* wp = dynamic_cast<const WorldObject*>(p);
        if (wp) {
            mat4f aux = glm::translate(mat4f(1.0f), wp->getPosition());
            aux = aux * glm::mat4_cast(wp->getRotation());
            mat4f orbTransform = glm::scale(aux, vec3f(rad));

            orbit.program->uniform("width")->set(radius/5.0f/rad);
            orbit.program->uniform("orbit")->set(orb);
            orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
            orbit.program->uniform("modelViewProjectionMatrix")->set(viewProjection*orbTransform);

            glDisable(GL_CULL_FACE);
            glDepthMask(GL_FALSE);
            orbit.draw();
            glDepthMask(GL_TRUE);
            glEnable(GL_CULL_FACE);
        }
    }
}
