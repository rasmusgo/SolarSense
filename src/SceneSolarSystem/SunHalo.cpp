#include <glm/gtc/matrix_inverse.hpp>
#include "SunHalo.hpp"
#include "Camera.hpp"

SunHalo::SunHalo() : time(0.0f) {
    this->setName("sunHalo");
    model.mesh = Meshes.get("square");
    model.program = Programs.get("sun");
}

SunHalo::~SunHalo(){

}


void SunHalo::update(float deltaTime) {
    time += deltaTime;
    transform = glm::rotate(mat4f(1.0f),90.f,vec3f(1,0,0));
}

void SunHalo::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

    mat4f projection = cam->projection;
    mat4f view = cam->view;

    vec3f camP = Utils::posFromMatrix(view);
    mat4f rot = glm::translate(mat4f(1.0f), -camP)*view;
    mat4f modelM = glm::transpose(rot)*glm::scale(fullTransform, vec3f(scale));
    mat4f t = projection*view*modelM;

    Texture* tex = Textures.get("sun");
    tex->bind();
    model.program->uniform("sampler")->set((int)tex->getSlot());
    model.program->uniform("modelViewProjectionMatrix")->set(t);
    model.program->uniform("globaltime")->set(time);
    // sphere.program->uniform("modelMatrix")->set(model);
    // sphere.program->uniform("viewMatrix")->set(view);


   glDisable(GL_CULL_FACE);
   model.draw();
   glEnable(GL_CULL_FACE);
}
