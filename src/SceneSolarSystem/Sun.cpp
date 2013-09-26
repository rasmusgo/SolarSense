#include "SceneSolarSystem.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include "../SolarSenseApp.hpp"
#include "Planet.hpp"
#include "Sun.hpp"

Sun::Sun(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed)
                                : Planet(parentScene, parentObject, scale, orbRadius, orbSpeed), parentObject(parentObject) {
    sphere.mesh = MeshManager::get("square");
    sphere.program = ShaderManager::get("sun");
}

Sun::~Sun(){

}


void Sun::update(float deltaTime) {
    mat4f m(1.0), temp;
    timeAcc += deltaTime;

    m = glm::translate(m, parentObject->pos); 
    // m = glm::rotate(m,timeAcc*orbSpeed,vec3f(0,1,0));
    
    // m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));

   
    baseMatrix = m;

     m = glm::scale(m,scale);

     
    //m = temp*m;


    


    // //pos = vec3f(m*vec4f(orbRadius, 0.f, 0.f, 0.f));

     m = glm::rotate(m,90.f,vec3f(1,0,0));
    

   
    sphere.modelMatrix = m;
}

void Sun::draw() const {
    drawFrom(mat4f(1.0f));
}

void Sun::drawFrom(mat4f from) const {
     mat4f projection = parentScene->getState().projection;
     mat4f view = parentScene->getState().view;
     mat4f model = from*glm::transpose(view)*sphere.modelMatrix;
     mat4f transform = projection*view*model;

     TextureManager::get("sun")->bind();
     sphere.program->uniform("sampler")->set(2);
     sphere.program->uniform("modelViewProjectionMatrix")->set(transform);
     sphere.program->uniform("globaltime")->set(GLOBALCLOCK.getElapsedTime().asSeconds());
     // sphere.program->uniform("modelMatrix")->set(model);
     // sphere.program->uniform("viewMatrix")->set(view);


    glDisable(GL_CULL_FACE);
    sphere.draw();
    glEnable(GL_CULL_FACE);
}

mat4f Sun::getModelMatrix() {
    return baseMatrix;
}
