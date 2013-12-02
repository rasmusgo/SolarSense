#include "Sun.hpp"
#include "Camera.hpp"
#include "Planet.hpp"

Sun::Sun(const std::string& name, float radius) : Planet(name, radius, 0.0f), time(0.0f) {
    this->setDrawPriority(-1);

    sphere.mesh = Meshes.get("spherehigh");
    sphere.program = Programs.get("sun3d");

    transform = mat4f(1.0f);
}

Sun::~Sun() {
}

void Sun::update(float deltaTime, float time) {
    this->time = time;

    WorldObject::update(deltaTime, time);
    Planet::update(deltaTime, time);
}

void Sun::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

    mat4f viewProjection = cam->projection*cam->view;
    mat4f t = viewProjection*glm::scale(fullTransform, getScale());

    Texture* tex = Textures.get("sun");
    tex->bind();
    sphere.program->uniform("sampler")->set((int)tex->getSlot());
    sphere.program->uniform("modelViewProjectionMatrix")->set(t);
    sphere.program->uniform("globaltime")->set(time);
    sphere.draw();
}
