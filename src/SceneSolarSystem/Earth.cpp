#include <glm/gtc/matrix_inverse.hpp>
#include "Planet.hpp"
#include "Earth.hpp"
#include "Camera.hpp"

Earth::Earth(const std::string& name, float radius, float orbRadius) : Planet(name, radius, orbRadius) {
    sphere.program = Programs.get("earthShader");

    atmo.mesh = Meshes.get("sphere");
    atmo.program = Programs.get("atmosphereShader");
    this->innerRadius = radius;
    this->outerRadius = radius *1.05f;
    mat4f rot = glm::mat4(1.0);
    //rotation = glm::rotate(glm::quat(rot), -27.5, vec3f(1,0,0));
}

Earth::~Earth(){

}

void Earth::update(float deltaTime, float time) {

    position = vec3f(orbRadius*cos(time*orbSpeed), 0.0f, orbRadius*sin(time*orbSpeed));

    rotation = glm::rotate(quat(), time*rotSpeed, vec3f(0,1,0)); //*axisRotation;

    // atmo->setPosition(position);

    WorldObject::update(deltaTime, time);
    Planet::update(deltaTime, time);
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

    vec3f lightPos = glm::normalize(vec3f(0.0f)-getPosition());// - position;
    float shininess = 20.0f;

//     vec3f lightPos = vec3f(0.0f);// - position;
//     float shininess = 25.0f;
// >>>>>>> cca6ae2e09879e044340e32981084012f45f652f
    vec3f emission = vec3f(0.1f);
    //vec3f specular = vec3f(0.5f);
    vec3f specular = vec3f(1.0f, 0.9255f, 0.698f)*0.6f;
    vec3f lightAmbient = vec3f(0.0f);
    vec3f lightDiffuse(1.0f);
    vec3f lightSpecular(1.0f);

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
    
    sphere.program->uniform("modelViewProjectionMatrix")->set(t);
    sphere.program->uniform("modelMatrix")->set(model);
    sphere.program->uniform("viewMatrix")->set(view);
    sphere.program->uniform("normalMatrix")->set(normalMatrix);

    sphere.draw();




    float Kr = 0.0025f;
    float Km = 0.0015f;
    float ESun = 20.f;
    float fScale = 1.f/(outerRadius-innerRadius);
    float fScaleDepth = 0.25f;
    float fCameraHeight = glm::length(cam->getPosition());
    float g = -0.95f; // Mie aerosol scattering constant
    float g2 = g*g;
    float wavelength = 0.65f;
    vec3f v3InvWavelength = vec3f(1 / pow(wavelength, 4));

    atmo.program->uniform("v3CameraPos")->set(cam->getPosition());       // The camera's current position
    atmo.program->uniform("v3LightPos")->set(vec3f(0.f));        // The direction vector to the light source
    atmo.program->uniform("v3InvWavelength")->set(v3InvWavelength);   // 1 / pow(wavelength, 4) for the red, green, and blue channels
    // atmo.program->uniform("fCameraHeight")->set(fCameraHeight); // The camera's current height
    atmo.program->uniform("fCameraHeight2")->set(fCameraHeight*fCameraHeight); // fCameraHeight^2
    atmo.program->uniform("fOuterRadius")->set(outerRadius);                   // The outer (atmosphere) radius
    atmo.program->uniform("fOuterRadius2")->set(outerRadius*outerRadius);      // fOuterRadius^2
    atmo.program->uniform("fInnerRadius")->set(innerRadius);                   // The inner (planetary) radius
    // atmo.program->uniform("fInnerRadius2")->set(innerRadius*innerRadius);      // fInnerRadius^2
    atmo.program->uniform("fKrESun")->set(Kr*ESun);                            // Kr * ESun
    atmo.program->uniform("fKmESun")->set(Km*ESun);                            // Kr * ESun
    atmo.program->uniform("fKr4PI")->set(Kr*4.f*3.14f);                           // Kr * 4 * PI
    atmo.program->uniform("fKm4PI")->set(Km*4.f*3.14f);                           // Km * 4 * PI
    atmo.program->uniform("fScale")->set(fScale);                              // 1 / (fOuterRadius - fInnerRadius)
    atmo.program->uniform("fScaleDepth")->set(fScaleDepth);                    // The scale depth (i.e. the altitude at which the atmosphere's average density is found)
    atmo.program->uniform("fScaleOverScaleDepth")->set(fScale / fScaleDepth);  // fScale / fScaleDepth
    // atmo.program->uniform("fExposure")->set(1.0f);  // fScale / fScaleDepth
    //atmo.program->uniform("modelMatrix")->set(glm::scale(fullTransform, vec3f(outerRadius)));
    atmo.program->uniform("viewMatrix")->set(view);
    atmo.program->uniform("projectionMatrix")->set(projection);

    // atmo.program->uniform("nSamples")->set(2);  
    // atmo.program->uniform("fSamples")->set(0.5f);  
   glDisable(GL_CULL_FACE);
    atmo.draw();
   glEnable(GL_CULL_FACE);
}
