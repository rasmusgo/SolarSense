#include "Scene.hpp"
#include "SolarSenseApp.hpp"

Scene::Scene(SolarSenseApp &parent) : GameObject(NULL,vec3f(0.0),vec3f(1.0)), parent(parent) {}
Scene::~Scene() {}
RenderState& Scene::getState() const { return state; }
