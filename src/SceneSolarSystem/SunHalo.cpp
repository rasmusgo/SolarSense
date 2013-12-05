#include <glm/gtc/matrix_inverse.hpp>
#include "SunHalo.hpp"
#include "Camera.hpp"

SunHalo::SunHalo(const std::string& name, float size) : time(0.0f) {
    this->setName(name);
    this->scale = vec3f(size);
    model.mesh = Meshes.get("square");
    model.program = Programs.get("sun");

    this->setDrawPriority(100); // We want to draw this the last object before the orbits
}

SunHalo::~SunHalo(){

}


void SunHalo::update(float deltaTime, float time) {
    this->time = time;
    transform = glm::rotate(mat4f(1.0f),90.f,vec3f(1,0,0));
    transform = glm::scale(transform, vec3f(getScale()));
}

void SunHalo::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

    mat4f projection = cam->projection;
    mat4f view = cam->view;

    mat4f modelM = cam->billboard(vec3f(0.0f)) * fullTransform;
    mat4f t = projection*view*modelM;

    Texture* tex = Textures.get("sunnoise");
    tex->bind();
    model.program->uniform("sampler")->set((int)tex->getSlot());
    model.program->uniform("modelViewProjectionMatrix")->set(t);
    model.program->uniform("globaltime")->set(time);
    // sphere.program->uniform("modelMatrix")->set(model);
    // sphere.program->uniform("viewMatrix")->set(view);


   glDisable(GL_CULL_FACE);
   glDepthMask(GL_FALSE);
   model.draw();
   glDepthMask(GL_TRUE);
   glEnable(GL_CULL_FACE);
}
