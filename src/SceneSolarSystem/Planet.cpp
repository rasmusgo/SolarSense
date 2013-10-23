#include "Planet.hpp"

Planet::Planet(const std::string& name)
    : radius(0.0f), orbRadius(0.0f), orbSpeed(0.0f), drawOrbit(true) {

    this->setName(name);
    sphere.mesh = Meshes.get("sphere");
    orbit.mesh = Meshes.get("square");
    orbit.program = Programs.get("orbit");
}

Planet::~Planet(){
}

void Planet::draw() const {
    if (parent =! NULL and drawOrbit) {
        float rad = orbRadius + radius;
        float orb = orbRadius/rad;
        mat4f orbTransform = glm::scale(parent->fullTransform, vec3f(rad));

        orbit.program->uniform("width")->set(radius/5.0f/rad);
        orbit.program->uniform("orbit")->set(orb);
        orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
        orbit.program->uniform("modelViewProjectionMatrix")->set(viewProjection*orbTransform);

        glDisable(GL_CULL_FACE);
        orbit.draw();
        glEnable(GL_CULL_FACE);
    }
}
