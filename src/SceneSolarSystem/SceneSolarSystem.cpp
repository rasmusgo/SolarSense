#include "SceneSolarSystem.hpp"
#include "../SolarSenseApp.hpp"
#include "TriangleObject.hpp"
#include "RegularPolygonObject.hpp"
#include "OrbitingObject.hpp"
#include "SphereObject.hpp"
#include "Planet.hpp"
#include "Earth.hpp"
#include "Sun.hpp"
#include "StandardPlanet.hpp"

#include "inputreader.h"

SceneSolarSystem::SceneSolarSystem(SolarSenseApp &parent) :
	Scene(parent),
    debugCounter(0.0), fpsCount(0), paused(false), stereoscopic3D(false) {

    readInput();

	//SCENE INIT
    std::cout << "* Loading new scene: SolarSystem" << std::endl;
	if (!loadResources()) {
        std::cout << "Could not load resources for SceneSolarSystem" << std::endl;
		parent.isRunning = false;
		return;
	}
	//Center mouse
    KeyAndMouseManager::setMousePos(SCRWIDTH/2,SCRHEIGHT/2,parent.getWindow());
    //Init Camera

    cam = new Camera(this, vec3f(0.0f,0.0f,30.0f));
    //add gameObjects

    //addObject(new       TriangleObject(this, vec3f( 10.0f, 0.0f,10.0f),   vec3f(0.1f)));
    //addObject(new RegularPolygonObject(this, vec3f(-10.0f, 0.0f,10.0f),   vec3f(1.0f), 6));

    stars = new SphereObject(this, vec3f(0.0f, 0.0f, 0.0f), vec3f(500.0f, 500.0f, 500.0f));
    addObject(stars);
    GameObject* center = new GameObject(this, vec3f(0.0f, 0.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f));
    addObject(center);


    StandardPlanet* mercury = new StandardPlanet(this, sun, vec3f(1.0f, 1.0f, 1.0f)*0.5f, 15, 5, "planetShader", "mercury");
    addObject("mercury",mercury);
    sun->addObject(mercury);

    StandardPlanet* venus = new StandardPlanet(this, sun, vec3f(1.0f, 1.0f, 1.0f)*0.4f, 20, 4, "planetShader", "venus");
    addObject("venus",venus);
    sun->addObject(venus);

    Earth* earth = new Earth(this, sun, vec3f(1.0f, 1.0f, 1.0f), 30, 3);
    addObject("earth",earth);
    sun->addObject(earth);


    StandardPlanet* moon = new StandardPlanet(this, earth, vec3f(1.0f, 1.0f, 1.0f)*0.2f, 3, 7, "planetShader", "moon");
    addObject("moon",moon);
    earth->addObject(moon);

    StandardPlanet* mars = new StandardPlanet(this, sun, vec3f(1.0f, 1.0f, 1.0f)*0.8f, 50, 2, "planetShader", "mars");
    addObject("mars",mars);
    sun->addObject(mars);

    StandardPlanet* jupiter = new StandardPlanet(this, sun, vec3f(1.0f, 1.0f, 1.0f)*4.f, 80, 1.5, "planetShader", "jupiter");
    addObject("jupiter",jupiter);
    sun->addObject(jupiter);

    OrbitingObject* sun = new OrbitingObject(this, center, vec3f(4.6f, 4.6f, 4.6f), 0, 0);
    addDrawableObject("sun",sun);

    Sun* sun2 = new Sun(this, center, vec3f(1.0f, 1.0f, 1.0f)*10.f, 0, 0);
    addObject("Sun",sun2);
    sun->addObject(sun2);


    currentObject = objectsOrder.begin();
    cam->setArround(objectsMap.at((*currentObject)));

	std::cout << "* Init done" << std::endl;
}

SceneSolarSystem::~SceneSolarSystem() {
	//SECENE CLOSE
    std::cout << "* Deleting GameObjects on SceneSolarSystem" << std::endl;
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
		delete *it;
}

void SceneSolarSystem::addObject(GameObject *obj) {
    GameObject::addObject(obj);
}

void SceneSolarSystem::addObject(const std::string &name, GameObject *obj) {
    addObject(obj);
    objectsMap.insert(std::pair<std::string, GameObject*>(name, obj));
    objectsOrder.push_back(name);
}

void SceneSolarSystem::addDrawableObject(const std::string &name, GameObject* dObj) {
    addObject(dObj);
    drawList.push_back(dObj);
    objectsMap.insert(std::pair<std::string, GameObject*>(name, dObj));
    objectsOrder.push_back(name);
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
    if(!ShaderManager::load("sun","data/shaders/sun.vert","data/shaders/sun.frag"))
        return false;
    if(!ShaderManager::load("sun3d","data/shaders/sun3d.vert","data/shaders/sun3d.frag"))
        return false;
    if(!ShaderManager::load("hand","data/shaders/hand.vert","data/shaders/hand.frag"))
        return false;

	//textures
    if(!TextureManager::load("cubetex","data/10x10tex.png",2))
		return false;
    if(!TextureManager::load("sun","data/SunTexture_2048.png",2))
        return false;
    if(!TextureManager::load("hand","data/hand_white.png",2))
        return false;

    /*
    * Earth ferran style
    */
     if(!ShaderManager::load("earthShader","data/shaders/earthShader.vert","data/shaders/earthShader.frag"))
         return false;

     if(!TextureManager::load("earth","data/earthmap.jpg",2))
         return false;
     if(!TextureManager::load("earthNight","data/lightsmap.jpg",3))
         return false;
     if(!TextureManager::load("earthWater","data/earthwatermap.png",4))
         return false;

    /*
    * Earth chris style
    */
    if(!ShaderManager::load("planetShader","data/shaders/planetShader.vert","data/shaders/planetShader.frag"))
        return false;
    if(!ShaderManager::load("planetShaderBump","data/shaders/planetShaderBump.vert","data/shaders/planetShaderBump.frag"))
        return false;
    if(!ShaderManager::load("earthtest","data/shaders/testshader.vert","data/shaders/testshader.frag"))
        return false;

//    if(!TextureManager::load("earth_daytime","data/earth_daytime.png",2))
//        return false;
//    if(!TextureManager::load("earth_nighttime","data/earth_nighttime.png",3))
//        return false;
//    if(!TextureManager::load("earth_cloud","data/earth_cloud.png",4))
//        return false;
//    if(!TextureManager::load("earth_specular","data/earth_specular.png",5))
//       return false;
    if(!TextureManager::load("stars4K","data/stars4K.png",2))
        return false;


    if(!TextureManager::load("moon","data/moonmap4k.jpg",2))
        return false;
    if(!TextureManager::load("moonbump","data/moonbump4k.jpg",3))
        return false;
    //if(!TextureManager::load("earth8k","data/earth8k.jpg",2))
    //    return false;
    if(!TextureManager::load("earthmap","data/earthmap.jpg",2))
        return false;
    if(!TextureManager::load("mars","data/marsmap1k.jpg",2))
        return false;
    if(!TextureManager::load("venus","data/venusmap.jpg",2))
        return false;

    if(!TextureManager::load("mercury","data/mercurymap.jpg",2))
        return false;
    if(!TextureManager::load("jupiter","data/jupitermap.jpg",2))
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
    static command cmd;
    m_cmd_q.lock();
    while(!cmd_q.empty()){
        cmd = cmd_q.front(); cmd_q.pop();
        if( ! cam->interpolating){
            switch(cmd.opcode){
                case '1':
                    if(--currentObject != objectsOrder.end()){
                        cam->setArround(objectsMap.at((*currentObject)));
                    }
                    break;
                case '2':
                    if(++currentObject != objectsOrder.end()){
                        cam->setArround(objectsMap.at((*currentObject)));
                    }
                    break;
                case '3':
                    cam->setMode(Camera::Free);
                    break;
            }
        }
    }
    m_cmd_q.unlock();


    //Update logic
    if (KeyAndMouseManager::isKeyPressed(sf::Keyboard::P)) paused = !paused;
    if (paused) deltaTime = 0.0f;
    if (not cam->interpolating && (KeyAndMouseManager::isKeyPressed(sf::Keyboard::Right) || SensorManager::checkGesture() == SensorManager::SWIPE_RIGHT)) {
        if (++currentObject != objectsOrder.end())
            cam->setArround(objectsMap.at((*currentObject)));
        else --currentObject;
    }
    if (not cam->interpolating && (KeyAndMouseManager::isKeyPressed(sf::Keyboard::Left) || SensorManager::checkGesture() == SensorManager::SWIPE_LEFT)) {
        if (currentObject != objectsOrder.begin())
            cam->setArround(objectsMap.at((*--currentObject)));
    }
    if (not cam->interpolating && (KeyAndMouseManager::isKeyPressed(sf::Keyboard::F) || SensorManager::checkGesture() == SensorManager::PUNCH)) cam->setMode(Camera::Free);
    if (not cam->interpolating && KeyAndMouseManager::isKeyPressed(sf::Keyboard::G)) cam->setMode(Camera::Arround);
    if (KeyAndMouseManager::isKeyPressed(sf::Keyboard::Num3)) {
        if (stereoscopic3D)
            glViewport(0,0,float(SCRWIDTH),float(SCRHEIGHT)); //back to normal
        stereoscopic3D = !stereoscopic3D;
    }
    if (KeyAndMouseManager::isKeyPressed(sf::Keyboard::R)) SensorManager::resetTracking();

    //Update Camera
    cam->update(deltaTime);

	for(std::list<GameObject*>::iterator it = objects.begin();it != objects.end(); ++it) {
		(*it)->update(deltaTime);
	}
	//Erase dead game objects
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
		if (!(*it)->isAlive) {
			delete *it;
            it = objects.erase(it);
		}
		else
			++it;

    //KeyAndMouseManager::setMousePos(SCRWIDTH/2,SCRHEIGHT/2,parent.getWindow());
}

void SceneSolarSystem::draw() const {
    if (not stereoscopic3D) {
        //calculate perspective matrix
        getState().projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);

        //Move matrix to position (according to player/camera)
        //getState().view = mat4f(1.0);
        getState().view = cam->getViewMatrix();

        //Drawable objects
        glDisable(GL_CULL_FACE);
        stars->draw();
        glEnable(GL_CULL_FACE);

        for(std::list<GameObject*>::const_iterator it = drawList.begin();it != drawList.end(); ++it)
            (*it)->draw();

        cam->drawHUD();
    }
    else {
        //calculate perspective matrix (Parallel)
        //getState().projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);

        //Move matrix to position (according to player/camera)
        //getState().view = mat4f(1.0);
        std::pair<mat4f,mat4f> eyes = cam->getViewMatrix3D();

        float ratio  = float(SCRWIDTH) / float(SCRHEIGHT);
        float radians = DEG_TO_RAD * FOV / 2.0;
        float wd2     = ZNEAR * glm::tan(radians);
        //float ndfl    = ZNEAR / ((ZFAR - ZNEAR)*0.5);
        float ndfl    = ZNEAR / 375.0;

        float left, right, top, bottom;

        glViewport(float(SCRWIDTH)/2.0,0,float(SCRWIDTH)/2.0,float(SCRHEIGHT)); //Right eye
        {
            //Off Axis projection
            left  = - ratio * wd2 - 0.5 * cam->eyeDistance3D * ndfl;
            right =   ratio * wd2 - 0.5 * cam->eyeDistance3D * ndfl;
            top    =   wd2;
            bottom = - wd2;
            getState().projection = glm::frustum(left,right,bottom,top,ZNEAR,ZFAR);

            getState().view = eyes.first;

            //Drawable objects
            glDisable(GL_CULL_FACE);
            stars->draw();
            glEnable(GL_CULL_FACE);

            for(std::list<GameObject*>::const_iterator it = drawList.begin();it != drawList.end(); ++it)
                (*it)->draw();

            cam->drawHUD();
        }
        glViewport(0,0,float(SCRWIDTH)/2.0,float(SCRHEIGHT)); //Left eye
        {
            //Off Axis projection
            left  = - ratio * wd2 + 0.5 * cam->eyeDistance3D * ndfl;
            right =   ratio * wd2 + 0.5 * cam->eyeDistance3D * ndfl;
            top    =   wd2;
            bottom = - wd2;
            getState().projection = glm::frustum(left,right,bottom,top,ZNEAR,ZFAR);

            getState().view = eyes.second;

            //Drawable objects
            glDisable(GL_CULL_FACE);
            stars->draw();
            glEnable(GL_CULL_FACE);

            for(std::list<GameObject*>::const_iterator it = drawList.begin();it != drawList.end(); ++it)
                (*it)->draw();

            cam->drawHUD();
        }
    }
}

