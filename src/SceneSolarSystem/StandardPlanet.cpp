#include "SceneSolarSystem.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include "../SolarSenseApp.hpp"
#include "Planet.hpp"
#include "StandardPlanet.hpp"

StandardPlanet::StandardPlanet(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed, const char* shaderprogram, const char* texture)
                                : Planet(parentScene, parentObject, scale, orbRadius, orbSpeed), parentObject(parentObject), shaderprogram(shaderprogram), texturemapname(texture) {
    sphere.mesh = MeshManager::get("sphere");
    sphere.program = ShaderManager::get(shaderprogram);
}
StandardPlanet::StandardPlanet(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed, const char* shaderprogram, const char* texture, const char* bumpmap)
                                : Planet(parentScene, parentObject, scale, orbRadius, orbSpeed), bumpmap(bumpmap), parentObject(parentObject), shaderprogram(shaderprogram), texturemapname(texture) {
    sphere.mesh = MeshManager::get("sphere");
    sphere.program = ShaderManager::get(shaderprogram);
}

StandardPlanet::~StandardPlanet(){

}


void StandardPlanet::update(float deltaTime) {
    mat4f m(1.0), temp;
    timeAcc += deltaTime;

    m = glm::rotate(m,timeAcc*orbSpeed,vec3f(0,1,0));
    
    m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));

   
    baseMatrix = parentObject->getModelMatrix()*m;

     m = glm::scale(m,scale);

    m = glm::rotate(m,timeAcc*orbSpeed*2,vec3f(0,1,0));
    

   
    sphere.modelMatrix = m;
}

void StandardPlanet::draw() const {
    drawFrom(mat4f(1.0f));
}

void StandardPlanet::drawFrom(mat4f from) const {
    mat4f modelViewProjectionMatrix = parentScene->getState().projection * parentScene->getState().view * from * sphere.modelMatrix;
    mat4f modelViewMatrix =  from*sphere.modelMatrix;
    mat4f normalMatrix( glm::transpose(glm::inverse(modelViewMatrix)));

    vec3f lightPos = vec3f(0,0,0); //parentObject->pos;

    Texture* sdp_tex = TextureManager::get(texturemapname);
    sdp_tex->bind();


    //if(*bumpmap != '\0')  {
    //    Texture* sdp_bump = TextureManager::get(bumpmap);
    //    sdp_bump->bind();
     //   sphere.program->uniform("bumpmap")->set(3); 

//    
    sphere.program->uniform("sampler")->set((int)sdp_tex->getSlot());


    sphere.program->uniform("LightPosition")->set(lightPos);
    sphere.program->uniform("MVPMatrix")->set(modelViewProjectionMatrix);
    sphere.program->uniform("MVMatrix")->set(modelViewMatrix);
    sphere.program->uniform("NormalMatrix")->set(normalMatrix);

    sphere.draw();


    for(std::list<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        Planet* p = dynamic_cast<Planet*>((*it));

        if (p != 0) {
            //Draw orbit of son
            //float rad = p->orbRadius + p->scale.x;
            //float orb = p->orbRadius/rad;
            //mat4f orbTransform = glm::scale(baseMatrix, vec3f(rad, rad, rad));

            // orbit.program->uniform("width")->set(p->scale.x/5.0f/rad);
            // orbit.program->uniform("orbit")->set(orb);
            // orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
            // orbit.program->uniform("modelViewProjectionMatrix")->set(viewProjection*orbTransform);
            // glDisable(GL_CULL_FACE);
            // orbit.draw();
            // glEnable(GL_CULL_FACE);

            p->drawFrom(from*baseMatrix);
        }
    }
}

mat4f StandardPlanet::getModelMatrix() {
    return baseMatrix;
}
