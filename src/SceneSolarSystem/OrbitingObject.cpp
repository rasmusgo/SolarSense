#include "OrbitingObject.hpp"
#include "Planet.hpp"
#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"

OrbitingObject::OrbitingObject(Scene* parentScene, GameObject* parentObject,
                               vec3f scale, float orbRadius, float orbSpeed)
                                : GameObject(parentScene, parentObject->pos, scale)
                                , orbRadius(orbRadius), orbSpeed(orbSpeed), parentObject(parentObject) {
    sph.mesh = MeshManager::get("sphere");
    sph.program = ShaderManager::get("sun3d");

    orbit.mesh = MeshManager::get("square");
    orbit.program = ShaderManager::get("orbit");
}

OrbitingObject::~OrbitingObject() {
}

void OrbitingObject::update(float deltaTime) {
    //(void) deltaTime;
    timeAcc += deltaTime;

    updateMatrix();
}

void OrbitingObject::updateMatrix() {
    mat4f m(1.0), temp;
    m = glm::rotate(m,timeAcc*orbSpeed,vec3f(0,1,0));
    
    m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));

   
    baseMatrix = m;

     m = glm::scale(m,scale);

     
    //m = temp*m;


    
    sph.modelMatrix = m;
}

void OrbitingObject::draw() const {
    drawFrom(mat4f(1.0f));
}

void OrbitingObject::drawFrom(mat4f from) const {
    mat4f viewProjection = parentScene->getState().projection*parentScene->getState().view;
    mat4f transform = viewProjection*from*sph.modelMatrix;
    TextureManager::get("sun")->bind();
    sph.program->uniform("sampler")->set(2);
    sph.program->uniform("modelViewProjectionMatrix")->set(transform);
    sph.program->uniform("globaltime")->set(GLOBALCLOCK.getElapsedTime().asSeconds());
    sph.draw();

    //Draw sons
    for(std::list<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        Planet* p = dynamic_cast<Planet*>((*it));

        if (p != 0) {
            //Draw orbit of son
            float rad = p->orbRadius + p->scale.x;
            float orb = p->orbRadius/rad;
            mat4f orbTransform = glm::scale(baseMatrix, vec3f(rad, rad, rad));

            orbit.program->uniform("width")->set(p->scale.x/5.0f/rad);
            orbit.program->uniform("orbit")->set(orb);
            orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
            orbit.program->uniform("modelViewProjectionMatrix")->set(viewProjection*orbTransform);           

            p->drawFrom(baseMatrix);

            glDisable(GL_CULL_FACE);
            orbit.draw();
            glEnable(GL_CULL_FACE);
        }
    }
}
