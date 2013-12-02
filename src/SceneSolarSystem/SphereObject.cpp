#include "SphereObject.hpp"
#include "Camera.hpp"

SphereObject::SphereObject() {
    sph.mesh = Meshes.get("sphere");
    sph.program = Programs.get("sample");

    transform = mat4f(1.0f);
}

SphereObject::~SphereObject() {
}

void SphereObject::update(float deltaTime, float time) {
    (void) deltaTime;
    (void) time;
}

void SphereObject::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
    mat4f t = cam->projection*cam->view*glm::scale(fullTransform, vec3f(radius));

    Texture* tex = Textures.get("stars4K");
    tex->bind();
    sph.program->uniform("sampler")->set((int)tex->getSlot());
    sph.program->uniform("modelViewProjectionMatrix")->set(t);
    glDisable(GL_CULL_FACE);
    sph.draw();
    glEnable(GL_CULL_FACE);
}

