#include <glm/gtc/matrix_inverse.hpp>
#include "Atmosphere.hpp"
#include "Camera.hpp"

#define PI 3.1415926535897932384626433832795f


Atmosphere::Atmosphere(const std::string& name, float radius, float orbRadius) : Planet(name, radius, orbRadius), time(0.0f){
    this->setName(name);
    this->innerRadius = radius;
    this->outerRadius = radius*1.025;
    this->scale = vec3f(outerRadius);

    atmo.mesh = Meshes.get("spherehigh");
    atmo.program = Programs.get("atmosphereShader");

    this->setDrawPriority(1000); // We want to draw this the last object before the orbits
}

Atmosphere::~Atmosphere(){

} 


void Atmosphere::update(float deltaTime) {
   time += deltaTime;

    position = vec3f(orbRadius*cos(time*orbSpeed), 0.0f, orbRadius*sin(time*orbSpeed));
    // rotation = glm::rotate(rotation, deltaTime*rotSpeed, vec3f(0,1,0)); //*axisRotation;

    // atmo->setPosition(position);

    WorldObject::update(deltaTime);
    Planet::update(deltaTime);
}


    vec3f mulm4(const mat4f& m, const vec3f& v){
        return (vec3f) ( m * vec4f(v, 0));
    }


void Atmosphere::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
    mat4f projection = cam->projection;
    mat4f view = cam->view;
    mat4f model = glm::scale(fullTransform, getScale());
    mat4f viewModel = glm::inverse(view*model);
    vec4f camPos = viewModel[3];
    mat4f iModel = ( (glm::inverse(model)));
    vec3f cameraPos = vec3f(camPos); //(vec3f) (iModel*(vec4f(cam->getPosition(), 0)));//****  //vec3f(model*vec4f(cam->getPosition(),1.0));// 
    float Kr = 0.00020f;
    float Km = 0.00015f;
    float ESun = 12.f;
    float fScale = 1.f/(outerRadius-innerRadius);
    float fScaleDepth = 0.25; //Must be 25%
    float fCameraHeight = glm::length(cameraPos-getLocalPosition());
    float g = -0.9810f; // Mie aerosol scattering constant
    float g2 = g*g;
    vec3f wavelength = vec3f(0.650f, 0.570f, 0.475f);
    vec3f v3InvWavelength = vec3f(1.0f / powf(wavelength.x, 4.0f), 1.0f / powf(wavelength.y, 4.0f), 1.0f / powf(wavelength.z, 4.0f));
    vec3f lightPos = glm::normalize(mulm4(iModel, -getPosition())); //vec3f(0.0f);//*******  //-vec3f(vec4f(cam->getPosition(),1.0f)*model); //vec3f(model*vec4f(vec3f(0.0),1.0));//
    // vec3f lightPos = vec3f(0.f);
    atmo.program->uniform("v3CameraPos")->set(cameraPos);       // The camera's current position
    atmo.program->uniform("v3LightPos")->set(lightPos);        // The direction vector to the light source
    atmo.program->uniform("v3InvWavelength")->set(v3InvWavelength);   // 1 / pow(wavelength, 4) for the red, green, and blue channels
    // atmo.program->uniform("fCameraHeight")->set(fCameraHeight); // The camera's current height
    atmo.program->uniform("fCameraHeight2")->set(fCameraHeight*fCameraHeight); // fCameraHeight^2
    atmo.program->uniform("fOuterRadius")->set(outerRadius);                   // The outer (atmosphere) radius
    atmo.program->uniform("fOuterRadius2")->set(outerRadius*outerRadius);      // fOuterRadius^2
    atmo.program->uniform("fInnerRadius")->set(innerRadius);                   // The inner (planetary) radius
    // atmo.program->uniform("fInnerRadius2")->set(innerRavec3f(0.0f);//*******  //dius*innerRadius);      // fInnerRadius^2
    atmo.program->uniform("fKrESun")->set(Kr*ESun);                            // Kr * ESun
    atmo.program->uniform("fKmESun")->set(Km*ESun);                            // Kr * ESun
    atmo.program->uniform("fKr4PI")->set(Kr*4.f*PI);                           // Kr * 4 * PI
    atmo.program->uniform("fKm4PI")->set(Km*4.f*PI);                           // Km * 4 * PI
    atmo.program->uniform("fScale")->set(fScale);                              // 1 / (fOuterRadius - fInnerRadius)
    atmo.program->uniform("fScaleDepth")->set(fScaleDepth);                    // The scale depth (i.e. the altitude at which the atmosphere's average density is found)
    atmo.program->uniform("fScaleOverScaleDepth")->set(fScale / fScaleDepth);  // fScale / fScaleDepth

    atmo.program->uniform("modelMatrix")->set(model);
    atmo.program->uniform("viewMatrix")->set(view);
    atmo.program->uniform("projectionMatrix")->set(projection);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    atmo.draw();    
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
}
