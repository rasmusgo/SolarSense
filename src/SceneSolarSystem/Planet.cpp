#include "Planet.hpp"
#include "Camera.hpp"

Planet::Planet(const std::string& name, float radius, float orbRadius)
    : radius(radius), orbRadius(orbRadius), orbSpeed(0.0f), drawOrbit(true) {

    this->setName(name);
    sphere.mesh = Meshes.get("sphere");

    orbit.mesh = Meshes.get("square");
    orbit.program = Programs.get("orbit");

    scale = vec3f(radius);
    position = vec3f(orbRadius, 0, 0);
}

Planet::~Planet(){
}

void Planet::update(float deltaTime) {
    (void) deltaTime;

    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

    float dist = glm::length(cam->getPosition() - getPosition());
    if (dist > 10.0*getScale().x) { // If we are far away we use the low poly sphere
        sphere.mesh = Meshes.get("spherelow");
    }
    else if (dist > 4.0*getScale().x) {
        sphere.mesh = Meshes.get("sphere");
    }
    else sphere.mesh = Meshes.get("spherehigh");
}

void Planet::draw() const {
    const GameObject* p = getParent();
    if (p and drawOrbit) {
        Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
        mat4f viewProjection = cam->projection*cam->view;

        float rad = orbRadius + radius;
        float orb = orbRadius/rad;
        //float distToCamera = glm::length(getPosition() - cam->getPosition());
        float width = radius/5.0f/rad;

        const WorldObject* wp = dynamic_cast<const WorldObject*>(p);
        if (wp) {
            mat4f aux = glm::translate(mat4f(1.0f), wp->getPosition());
            aux = aux * glm::mat4_cast(wp->getRotation());
            mat4f orbTransform = glm::scale(aux, vec3f(rad));

            orbit.program->uniform("width")->set(width);
            orbit.program->uniform("orbit")->set(orb);
            orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
            orbit.program->uniform("modelMatrix")->set(orbTransform);
            orbit.program->uniform("modelViewProjectionMatrix")->set(viewProjection*orbTransform);
            orbit.program->uniform("planetPos")->set(getPosition());
            orbit.program->uniform("radius")->set(orbRadius);

            glDisable(GL_CULL_FACE);
            glDepthMask(GL_FALSE);
            orbit.draw();
            glDepthMask(GL_TRUE);
            glEnable(GL_CULL_FACE);
        }
    }
}
