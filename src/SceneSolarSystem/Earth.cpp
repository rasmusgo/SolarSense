#include <glm/gtc/matrix_inverse.hpp>
#include "Planet.hpp"
#include "Earth.hpp"
#include "Camera.hpp"

#define PI 3.1415926535897932384626433832795f


Earth::Earth(const std::string& name, float radius, float orbRadius) : Planet(name, radius, orbRadius), time(0.0f) {
    sphere.program = Programs.get("earthShader");

    sphere.mesh = Meshes.get("sphere");
    // sphere.program = Programs.get("atmosphereShader");
    this->innerRadius = radius;
    this->outerRadius = radius *1.025f;
    mat4f rot = glm::mat4(1.0);
    this->setDrawPriority(100); // We want to draw this the last object before the orbits

    rotation = glm::rotate(glm::quat(rot), -27.5, vec3f(1,0,0));
}

Earth::~Earth(){

}

void Earth::update(float deltaTime, float time) {
    this->time = time;

    position = vec3f(orbRadius*cos(time*orbSpeed), 0.0f, orbRadius*sin(time*orbSpeed));

    rotation = glm::rotate(quat(), time*rotSpeed, vec3f(0,1,0)); //*axisRotation;

    // sphere->setPosition(position);

    WorldObject::update(deltaTime, time);
    Planet::update(deltaTime, time);
}

    vec3f mul4(const mat4f& m, const vec3f& v){
        return (vec3f) ( m * vec4f(v, 0));
    }


void Earth::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));

//    mat4f t = glm::scale(fullTransform, vec3f(radius));
//    mat4f modelViewProjectionMatrix = cam->projection * cam->view * t;
//    mat4f modelViewMatrix =  t;
//    mat4f normalMatrix( glm::transpose(glm::inverse(modelViewMatrix)));

//    vec3f lightPos = vec3f(0,0,0); // parentObject->pos;

//    Texture* earth_day = TextureManager::get("earth_daytime");
//    earth_day->bind();
//    sphere.program->uniform("sampler")->set(2);

//    Texture* earth_night = TextureManager::get("earth_nighttime");
//    earth_night->bind();
//    sphere.program->uniform("EarthNight")->set(3);

//    Texture* earth_cloud = TextureManager::get("earth_cloud");
//    earth_cloud->bind();
//    sphere.program->uniform("EarthCloudGloss")->set(4);

//     Texture* earth_specular_map = TextureManager::get("earth_specular");
//     earth_specular_map->bind();
//     sphere.program->uniform("EarthSpecularMap")->set(5);

//    sphere.program->uniform("LightPosition")->set(lightPos);
//    sphere.program->uniform("MVPMatrix")->set(modelViewProjectionMatrix);
//    sphere.program->uniform("MVMatrix")->set(modelViewMatrix);
//    sphere.program->uniform("NormalMatrix")->set(normalMatrix);

    mat4f projection = cam->projection;
    mat4f view = cam->view;
    mat4f model = glm::scale(fullTransform, getScale());
    mat4f t = projection*view*model;
    mat4f normalMatrix( glm::transpose(glm::inverse(model)));

    vec3f lightPos = vec3f(0.0f);// - position;
    float shininess = 10.0f;

    vec3f emission = vec3f(0.0f);
    //vec3f specular = vec3f(0.5f);
    vec3f specular = vec3f(1.0f, 0.9255f, 0.698f)*0.3f;
    vec3f lightAmbient = vec3f(0.0f);
    vec3f lightDiffuse(1.0f);
    vec3f lightSpecular(1.0f);

    mat4f viewModel = glm::inverse(view*model);
    vec4f camPos = viewModel[3];
    mat4f iModel = ( (glm::inverse(model)));
    vec3f cameraPos = vec3f(camPos); //(vec3f) (iModel*(vec4f(cam->getPosition(), 0)));//****  //vec3f(model*vec4f(cam->getPosition(),1.0));// 
    float Kr = 0.0025f;
    float Km = 0.0010f;
    float ESun = 2.f;
    float fScale = 1.f/(outerRadius-innerRadius);
    float fScaleDepth = 0.25f; //Must be 25%
    float fCameraHeight = glm::length(cameraPos);
    float g = -0.750f; // Mie aerosol scattering constant
    float g2 = g*g;
    vec3f wavelength = vec3f(0.650f, 0.570f, 0.475f);
    vec3f v3InvWavelength = vec3f(1.0f / powf(wavelength.x, 4.0f), 1.0f / powf(wavelength.y, 4.0f), 1.0f / powf(wavelength.z, 4.0f));
    vec3f lightPos_v2 = glm::normalize(mul4(iModel, -getPosition()));
    // vec3f lightPos = vec3f(0.f);
    sphere.program->uniform("v3CameraPos")->set(cameraPos);       // The camera's current position
    sphere.program->uniform("v3LightPos")->set(lightPos_v2);        // The direction vector to the light source
    sphere.program->uniform("v3InvWavelength")->set(v3InvWavelength);   // 1 / pow(wavelength, 4) for the red, green, and blue channels
    // sphere.program->uniform("fCameraHeight")->set(fCameraHeight); // The camera's current height
    sphere.program->uniform("fCameraHeight2")->set(fCameraHeight*fCameraHeight); // fCameraHeight^2
    sphere.program->uniform("fOuterRadius")->set(outerRadius);                   // The outer (spheresphere) radius
    sphere.program->uniform("fOuterRadius2")->set(outerRadius*outerRadius);      // fOuterRadius^2
    sphere.program->uniform("fInnerRadius")->set(innerRadius);                   // The inner (planetary) radius
    // sphere.program->uniform("fInnerRadius2")->set(innerRavec3f(0.0f);//*******  //dius*innerRadius);      // fInnerRadius^2
    sphere.program->uniform("fKrESun")->set(Kr*ESun);                            // Kr * ESun
    sphere.program->uniform("fKmESun")->set(Km*ESun);                            // Kr * ESun
    sphere.program->uniform("fKr4PI")->set(Kr*4.f*PI);                           // Kr * 4 * PI
    sphere.program->uniform("fKm4PI")->set(Km*4.f*PI);                           // Km * 4 * PI
    sphere.program->uniform("fScale")->set(fScale);                              // 1 / (fOuterRadius - fInnerRadius)
    sphere.program->uniform("fScaleDepth")->set(fScaleDepth);                    // The scale depth (i.e. the altitude at which the spheresphere's average density is found)
    sphere.program->uniform("fScaleOverScaleDepth")->set(fScale / (fScaleDepth) );  // fScale / fScaleDepth


    sphere.program->uniform("lightPos")->set(lightPos);
    sphere.program->uniform("shininess")->set(shininess);
    sphere.program->uniform("emission")->set(emission);
    sphere.program->uniform("specular")->set(specular);
    sphere.program->uniform("lightAmbient")->set(lightAmbient);
    sphere.program->uniform("lightDiffuse")->set(lightDiffuse);
    //sphere.program->uniform("lightSpecular")->set(lightSpecular);

    Texture* tex;
    tex = Textures.get("earth");
    tex->bind();
    sphere.program->uniform("sampler")->set((int)tex->getSlot());

    tex = Textures.get("earthNight");
    tex->bind();
    sphere.program->uniform("samplerNight")->set((int)tex->getSlot());

    tex = Textures.get("earthWater");
    tex->bind();
    sphere.program->uniform("samplerWater")->set((int)tex->getSlot());

    tex = Textures.get("earthWaterTex");
    tex->bind();
    sphere.program->uniform("samplerWaterTex")->set((int)tex->getSlot());


    tex = Textures.get("earthNormal");
    tex->bind();
    sphere.program->uniform("samplerNormal")->set((int)tex->getSlot());
    
    tex = Textures.get("earthClouds");
    tex->bind();
    sphere.program->uniform("samplerCloud")->set((int)tex->getSlot());
    
    sphere.program->uniform("modelViewProjectionMatrix")->set(t);
    sphere.program->uniform("modelMatrix")->set(model);
    sphere.program->uniform("viewMatrix")->set(view);
    sphere.program->uniform("normalMatrix")->set(normalMatrix);
    sphere.program->uniform("globaltime")->set(time);


    //glDisable(GL_CULL_FACE);
    //glDepthMask(GL_FALSE);
    sphere.draw();
    //glDepthMask(GL_TRUE);
    //glEnable(GL_CULL_FACE);


}
