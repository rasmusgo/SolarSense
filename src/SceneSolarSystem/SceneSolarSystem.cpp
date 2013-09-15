#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"
#include "TriangleObject.hpp"
#include "RegularPolygonObject.hpp"
#include "OrbitingObject.hpp"
#include "Planet.hpp"
#include "Earth.hpp"

SceneSolarSystem::SceneSolarSystem(SolarSenseApp &parent) :
	Scene(parent),
	debugCounter(0.0), fpsCount(0) {

	//SCENE INIT
    std::cout << "* Loading new scene: SolarSystem" << std::endl;
	if (!loadResources()) {
        std::cout << "Could not load resources for SceneSolarSystem" << std::endl;
		parent.isRunning = false;
		return;
	}
	//Center mouse
	InputManager::setMousePos(SCRWIDTH/2,SCRHEIGHT/2,parent.getWindow());
    //Init Camera

    cam = new Camera(this, vec3f(0.0f,3.0f,50.0f));
    //add gameObjects

    //addObject(new       TriangleObject(this, vec3f( 10.0f, 0.0f,10.0f),   vec3f(0.1f)));
    //addObject(new RegularPolygonObject(this, vec3f(-10.0f, 0.0f,10.0f),   vec3f(1.0f), 6));


    GameObject* center = new GameObject(this, vec3f(0.0f, 0.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f));
    addObject(center);
    // OrbitingObject* sun = new OrbitingObject(this, center, vec3f(10.0f, 10.0f, 10.0f), 0, 0);
    // addDrawableObject(sun);

    Earth* earth = new Earth(this, center, vec3f(15.0f, 15.0f, 15.0f), 0, 0);
    addDrawableObject(earth);

	std::cout << "* Init done" << std::endl;
}

SceneSolarSystem::~SceneSolarSystem() {
	//SECENE CLOSE
    std::cout << "* Deleting GameObjects on SceneSolarSystem" << std::endl;
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
		delete *it;
}

void SceneSolarSystem::addDrawableObject(GameObject* dObj) {
    addObject(dObj);
    drawList.push_back(dObj);
}

bool SceneSolarSystem::loadResources() {
	//shaders
    if(!ShaderManager::load("sample","data/shaders/sample.vert","data/shaders/sample.frag"))
		return false;
    if(!ShaderManager::load("orbit","data/shaders/orbit.vert","data/shaders/orbit.frag"))
        return false;
	if(!ShaderManager::load("sample2","data/shaders/sample2.vert","data/shaders/sample2.frag"))
		return false;
	if(!ShaderManager::load("earthtest","data/shaders/testshader.vert","data/shaders/testshader.frag"))
		return false;

	//textures
    if(!TextureManager::load("cubetex","data/10x10tex.png",2))
		return false;
    if(!TextureManager::load("sun","data/SunTexture_2048.png",2))
        return false;
    if(!TextureManager::load("earth","data/earth.png",2))
        return false;

	//Create meshes
	MeshManager::add("cube",new Mesh("data/10x10.obj"));
    MeshManager::add("sphere",new Mesh("data/sphere.obj"));
    MeshManager::add("square",new Mesh("data/square.obj"));

	return true;
}

void SceneSolarSystem::update(float deltaTime) {

	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
        std::cout << "FPS: " << fpsCount << ". Amount of GameObjects: " << objects.size() << ". Amount of Drawables: " << drawList.size() << std::endl;
		debugCounter -= 1;
		fpsCount = 0;
	}

    //Update Camera
    cam->update(deltaTime);

	for(std::list<GameObject*>::iterator it = objects.begin();it != objects.end(); ++it) {
		(*it)->update(deltaTime);
	}
	//Erase dead game objects
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
		if (!(*it)->isAlive) {
			delete *it;
			it = objects.erase(it);;
		}
		else
			++it;
}

void SceneSolarSystem::draw() const {
	//calculate perspective matrix
	getState().projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);

    //Move matrix to position (according to player/camera)
    //getState().view = mat4f(1.0);
    getState().view = cam->getViewMatrix();

    //Drawable objects
    for(std::list<GameObject*>::const_iterator it = drawList.begin();it != drawList.end(); ++it)
        (*it)->draw();
}

