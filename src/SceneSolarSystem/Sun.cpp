#include "Sun.hpp"
#include "Camera.hpp"
#include "Planet.hpp"

Sun::Sun() : time(0.0f), radius(0.0f) {
    this->setName("sun");
    this->setDrawPriority(-1);

    sph.mesh = Meshes.get("sphere");
    sph.program = Programs.get("sun3d");

    transform = mat4f(1.0f);
}

Sun::~Sun() {
}

void Sun::update(float deltaTime) {
    time += deltaTime;
}

void Sun::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

    mat4f viewProjection = cam->projection*cam->view;
    mat4f t = viewProjection*glm::scale(fullTransform, vec3f(radius));

    Texture* tex = Textures.get("sun");
    tex->bind();
    sph.program->uniform("sampler")->set((int)tex->getSlot());
    sph.program->uniform("modelViewProjectionMatrix")->set(t);
    sph.program->uniform("globaltime")->set(time);
    sph.draw();
}
