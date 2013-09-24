#include "SceneSolarSystem.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include "../SolarSenseApp.hpp"
#include "Planet.hpp"
#include "Earth.hpp"

Earth::Earth(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed)
                                : Planet(parentScene, parentObject, scale, orbRadius, orbSpeed), parentObject(parentObject) {
    sphere.mesh = MeshManager::get("sphere");
    sphere.program = ShaderManager::get("earthtest");
}

Earth::~Earth(){

}


void Earth::update(float deltaTime) {
    (void) deltaTime;
    mat4f m(1.0), temp;


    //m = glm::translate(m, parentObject->pos); 
    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed,vec3f(0,1,0));
    
    m = glm::translate(m,vec3f(orbRadius, 0.0f, 0.0f));

   
    baseMatrix = m;

     m = glm::scale(m,scale);

     
    //m = temp*m;


    


    //pos = vec3f(m*vec4f(orbRadius, 0.f, 0.f, 0.f));

    m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*orbSpeed*2,vec3f(0,1,0));
    

   
    sphere.modelMatrix = m;
}

void Earth::draw() const {
    drawFrom(mat4f(1.0f));
}

void Earth::drawFrom(mat4f from) const {
    mat4f modelViewProjectionMatrix = parentScene->getState().projection * parentScene->getState().view * from*sphere.modelMatrix;
    mat4f modelViewMatrix =  from*sphere.modelMatrix;
    mat4f normalMatrix( glm::transpose(glm::inverse(modelViewMatrix)));

    vec3f lightPos = vec3f(0,0,0); // parentObject->pos;

    Texture* earth_day = TextureManager::get("earth_daytime");
    earth_day->bind();
    sphere.program->uniform("sampler")->set(2);   

    Texture* earth_night = TextureManager::get("earth_nighttime");
    earth_night->bind();
    sphere.program->uniform("EarthNight")->set(3);   

    Texture* earth_cloud = TextureManager::get("earth_cloud");
    earth_cloud->bind();
    sphere.program->uniform("EarthCloudGloss")->set(4);   

    // Texture* earth_specular_map = TextureManager::get("earth_specular");
    // earth_specular_map->bind();
    // sphere.program->uniform("EarthSpecularMap")->set(5);   

    sphere.program->uniform("LightPosition")->set(lightPos);
    sphere.program->uniform("MVPMatrix")->set(modelViewProjectionMatrix);
    sphere.program->uniform("MVMatrix")->set(modelViewMatrix);
    sphere.program->uniform("NormalMatrix")->set(normalMatrix);


    // mat4f projection = parentScene->getState().projection;
    // mat4f view = parentScene->getState().view;
    // mat4f model = from*sphere.modelMatrix;
    // mat4f transform = projection*view*model;

    // vec3f lightPos = vec3f(0.0f);
    // float shininess = 15.0f;
    // vec3f emission = vec3f(0.1f);
    // vec3f specular = vec3f(1.0f);
    // vec3f lightAmbient = vec3f(1.0f);
    // vec3f lightDiffuse(1.0f);
    // vec3f lightSpecular(1.0f);

    // sphere.program->uniform("lightPos")->set(lightPos);
    // sphere.program->uniform("shininess")->set(shininess);
    // sphere.program->uniform("emission")->set(emission);
    // sphere.program->uniform("specular")->set(specular);
    // sphere.program->uniform("lightAmbient")->set(lightAmbient);
    // sphere.program->uniform("lightDiffuse")->set(lightDiffuse);
    // sphere.program->uniform("lightSpecular")->set(lightSpecular);

    // TextureManager::get("earth")->bind();
    // sphere.program->uniform("sampler")->set(2);
    // TextureManager::get("earthNight")->bind();
    // sphere.program->uniform("samplerNight")->set(3);
    // TextureManager::get("earthWater")->bind();
    // sphere.program->uniform("samplerWater")->set(4);
    // sphere.program->uniform("modelViewProjectionMatrix")->set(transform);
    // sphere.program->uniform("modelMatrix")->set(model);
    // sphere.program->uniform("viewMatrix")->set(view);


    sphere.draw();


    for(std::list<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        Planet* p = dynamic_cast<Planet*>((*it));

        if (p != 0) {
            //Draw orbit of son
            float rad = p->orbRadius + p->scale.x;
            float orb = p->orbRadius/rad;
            mat4f orbTransform = glm::scale(baseMatrix, vec3f(rad, rad, rad));

            //orbit.program->uniform("width")->set(p->scale.x/5.0f/rad);
            //orbit.program->uniform("orbit")->set(orb);
            //orbit.program->uniform("color")->set(vec3f(1.0, 1.0, 1.0));
            //orbit.program->uniform("modelViewProjectionMatrix")->set(from*baseMatrix*orbTransform);
            //glDisable(GL_CULL_FACE);
            //orbit.draw();
            //alEnable(GL_CULL_FACE);

            p->drawFrom(from*baseMatrix);
        }
    }
}
