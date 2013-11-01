#include <glm/gtc/matrix_inverse.hpp>
#include "Atmosphere.hpp"
#include "Camera.hpp"

Atmosphere::Atmosphere(const std::string& name, float planetRadius, float outerRadius) : time(0.0f) {
    this->setName(name);
    this->scale = vec3f(outerRadius);
    this->outerRadius = outerRadius;
    this->innerRadius = planetRadius;

    atmo.program = Programs.get("atmosphereShader");

    this->setDrawPriority(100); // We want to draw this the last object before the orbits
}

Atmosphere::~Atmosphere(){

}


void Atmosphere::update(float deltaTime) {
    time += deltaTime;
    transform = glm::rotate(mat4f(1.0f),90.f,vec3f(1,0,0));
    transform = glm::scale(transform, vec3f(getScale()));
}

void Atmosphere::draw() const {
    Camera* cam = static_cast<Camera*>(getGame()->getObjectByName("cam"));
    mat4f projection = cam->projection;
    mat4f view = cam->view;
    mat4f model = glm::scale(fullTransform, getScale());
    mat4f t = projection*view*modelM;

    Texture* tex = Textures.get("sun");
    tex->bind();

    float Kr = 0.5f;
    float Km = 0.5f;
    float ESun = 5.f;
    float fScale = 1.f/(outerRadius-innerRadius);
    float fScaleDepth = 0.25f;
    float fCameraHeight = (float) cam->getPosition().y;
    float g = -0.75f; // Mie aerosol scattering constant
    float g2 = g*g;
    float wavelength = 100.f;
    vec3f v3InvWavelength = vec3f(1 / pow(wavelength, 4));

    model.program->uniform("v3CameraPos")->set(cam->getPosition());       // The camera's current position
    model.program->uniform("v3LightPos")->set(vec3f(0.f));        // The direction vector to the light source
    model.program->uniform("v3InvWavelength")->set(v3InvWavelength);   // 1 / pow(wavelength, 4) for the red, green, and blue channels
    model.program->uniform("fCameraHeight")->set(fCameraHeight); // The camera's current height
    model.program->uniform("fCameraHeight2")->set(fCameraHeight*fCameraHeight); // fCameraHeight^2
    model.program->uniform("fOuterRadius")->set(outerRadius);                   // The outer (atmosphere) radius
    model.program->uniform("fOuterRadius2")->set(outerRadius*outerRadius);      // fOuterRadius^2
    model.program->uniform("fInnerRadius")->set(innerRadius);                   // The inner (planetary) radius
    model.program->uniform("fInnerRadius2")->set(innerRadius*innerRadius);      // fInnerRadius^2
    model.program->uniform("fKrESun")->set(Kr*ESun);                            // Kr * ESun
    model.program->uniform("fKr4PI")->set(Kr*4.f*3.14f);                           // Kr * 4 * PI
    model.program->uniform("fKm4PI")->set(Km*4.f*3.14f);                           // Km * 4 * PI
    model.program->uniform("fScale")->set(fScale);                              // 1 / (fOuterRadius - fInnerRadius)
    model.program->uniform("fScaleDepth")->set(fScaleDepth);                    // The scale depth (i.e. the altitude at which the atmosphere's average density is found)
    model.program->uniform("fScaleOverScaleDepth")->set(fScale / fScaleDepth);  // fScale / fScaleDepth
    model.program->uniform("nSamples")->set(2);  
    model.program->uniform("fSamples")->set(0.5f);  

    // model.program->uniform("modelViewProjectionMatrix")->set(t);
    // model.program->uniform("globaltime")->set(time);
    // sphere.program->uniform("modelMatrix")->set(model);
    // sphere.program->uniform("viewMatrix")->set(view);


   glDisable(GL_CULL_FACE);
   glDepthMask(GL_FALSE);
   model.draw();
   glDepthMask(GL_TRUE);
   glEnable(GL_CULL_FACE);
}
