#include "SceneSolarSystem.hpp"
#include "Camera.hpp"
#include "Sun.hpp"
#include "Planet.hpp"
#include "Earth.hpp"
#include "SunHalo.hpp"
#include "StandardPlanet.hpp"
#include "input/SensorManager.hpp"

#include "inputreader.h"

SceneSolarSystem::SceneSolarSystem() :
    debugCounter(0.0), fpsCount(0), paused(false), stereoscopic3D(false) {
    this->setName("SCENE");
    this->setDrawPriority(9001); // want to draw 3d on the last place

    readInput();

    //SCENE INIT
    if (!loadResources())
        VBE_ASSERT(false,"Could not load resources for SceneSolarSystem");

    //GL stuff..
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_CULL_FACE); //enable backface culling
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

	//Center mouse
    Input::setMousePos(SCRWIDTH/2,SCRHEIGHT/2,getGame()->getWindow());

    //Init Camera
    cam = new Camera(vec3f(0.0f,3.0f,30.0f));
    cam->addTo(this);

    //add gameObjects
    stars = new SphereObject();
    stars->radius = 500.0f;
    stars->setDrawPriority(-10);
    stars->addTo(this);

    Sun* sun = new Sun("sun", 4.7f);
    sun->rotSpeed = 1.0f;
    sun->addTo(this);
    objectsOrder.push_back("sun");

    float fa = 80.0f;
    StandardPlanet* mercury = new StandardPlanet("mercury", 0.5f, 15.0f ,"planetShader", "mercury");
    mercury->orbSpeed = 6.0f/fa;
    mercury->rotSpeed = 6.0f;
    mercury->addTo(sun);
    objectsOrder.push_back("mercury");

    StandardPlanet* venus = new StandardPlanet("venus", 0.4f, 20.0f,"planetShader","venus");
    venus->orbSpeed = 4.0f/fa;
    venus->rotSpeed = 4.0f;
    venus->addTo(sun);
    objectsOrder.push_back("venus");

    Earth* earth = new Earth("earth", 1.0f, 30.0f);
    earth->orbSpeed = 3.0f/fa;
    earth->rotSpeed = 3.0f;
    earth->addTo(sun);
    objectsOrder.push_back("earth");

    StandardPlanet* moon = new StandardPlanet("moon", 0.2f, 3.0f, "planetShader", "moon");
    moon->orbSpeed = 3.0f/fa;
    //moon->rotSpeed = 3.0f/8.0f;
    moon->addTo(earth);
    objectsOrder.push_back("moon");

    StandardPlanet* mars = new StandardPlanet("mars", 0.8f, 50.0f, "planetShader", "mars");
    mars->orbSpeed = 2.0f/fa;
    mars->rotSpeed = 2.0f;
    mars->addTo(sun);
    objectsOrder.push_back("mars");

    StandardPlanet* jupiter = new StandardPlanet("jupiter", 4.0f, 80.0f, "planetShader", "jupiter");
    jupiter->orbSpeed = 1.5f/fa;
    jupiter->rotSpeed = 1.5f;
    jupiter->addTo(sun);
    objectsOrder.push_back("jupiter");

    SunHalo* sunhalo = new SunHalo("sunHalo", 10.0f);
    sunhalo->addTo(sun);

    currentObject = objectsOrder.begin();
    cam->setArround((*currentObject));

	std::cout << "* Init done" << std::endl;
}

SceneSolarSystem::~SceneSolarSystem() {
    Textures.clear();
    Meshes.clear();
    Programs.clear();

    AudioManager::clear();
}

bool SceneSolarSystem::loadResources() {

    ShaderProgram* p;
    Texture* tex;

    VBE_LOG("Loading resources..");

	//shaders
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/sample.vert","data/shaders/sample.frag");
    Programs.add("sample",p);
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/orbit.vert","data/shaders/orbit.frag");
    Programs.add("orbit",p);
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/sample2.vert","data/shaders/sample2.frag");
    Programs.add("sample2",p);
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/sun.vert","data/shaders/sun.frag");
    Programs.add("sun",p);
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/sun3d.vert","data/shaders/sun3d.frag");
    Programs.add("sun3d",p);
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/hand.vert","data/shaders/hand.frag");
    Programs.add("hand",p);

    // Earth Ferran Style
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/earthShader.vert","data/shaders/earthShader.frag");
    Programs.add("earthShader",p);

    // Earth Chris style
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/planetShader.vert","data/shaders/planetShader.frag");
    Programs.add("planetShader",p);
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/planetShaderBump.vert","data/shaders/planetShaderBump.frag");
    Programs.add("planetShaderBump",p);
    p = new ShaderProgram();
    p->makeProgramFromFile("data/shaders/testshader.vert","data/shaders/testshader.frag");
    Programs.add("earthtest",p);


    VBE_LOG("Shaders Loaded");


	//textures
    tex = new Texture(1);
    if(!tex->loadFromFile("data/10x10tex.png",true)) return false;
    Textures.add("cubetex", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/SunTexture_2048.png",true)) return false;
    Textures.add("sun", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/hand_white.png",true)) return false;
    Textures.add("hand", tex);

    // Earth Ferran style
    tex = new Texture(1);
    if(!tex->loadFromFile("data/earthmap.jpg",true)) return false;
    Textures.add("earth", tex);
    tex = new Texture(2);
    if(!tex->loadFromFile("data/lightsmap.jpg",true)) return false;
    Textures.add("earthNight", tex);
    tex = new Texture(3);
    if(!tex->loadFromFile("data/earthwatermap.png",true)) return false;
    Textures.add("earthWater", tex);
    //Earth Chris style
//    if(!TextureManager::load("earth_daytime","data/earth_daytime.png",2))
//        return false;
//    if(!TextureManager::load("earth_nighttime","data/earth_nighttime.png",3))
//        return false;
//    if(!TextureManager::load("earth_cloud","data/earth_cloud.png",4))
//        return false;
//    if(!TextureManager::load("earth_specular","data/earth_specular.png",5))
//       return false;

    tex = new Texture(1);
    if(!tex->loadFromFile("data/stars4K.png",true)) return false;
    Textures.add("stars4K", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/moonmap4k.jpg",true)) return false;
    Textures.add("moon", tex);
    tex = new Texture(2);
    if(!tex->loadFromFile("data/moonbump4k.jpg",true)) return false;
    Textures.add("moonbump", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/earthmap.jpg",true)) return false;
    Textures.add("earthmap", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/marsmap1k.jpg",true)) return false;
    Textures.add("mars", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/venusmap.jpg",true)) return false;
    Textures.add("venus", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/mercurymap.jpg",true)) return false;
    Textures.add("mercury", tex);
    tex = new Texture(1);
    if(!tex->loadFromFile("data/jupitermap.jpg",true)) return false;
    Textures.add("jupiter", tex);


    VBE_LOG("Textures Loaded");


    //Create meshes
    Meshes.add("cube",new Mesh("data/10x10.obj"));
    Meshes.add("sphere",new Mesh("data/bola.obj"));
    Meshes.add("spherelow",new Mesh("data/sphere.obj"));
    Meshes.add("square",new Mesh("data/square.obj"));


    VBE_LOG("Meshes Loaded");

	return true;
}

void SceneSolarSystem::update(float deltaTime) {

	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
        std::cout << "FPS: " << fpsCount << ". Amount of GameObjects: " << getGame()->getObjectCount() << std::endl;
		debugCounter -= 1;
		fpsCount = 0;
	}

    // Comunication outside
    static command cmd;
    m_cmd_q.lock();
    while(!cmd_q.empty()){
        cmd = cmd_q.front(); cmd_q.pop();
        switch(cmd.opcode){
            case 1: //Planet switch commands
            {
                int action;
                sscanf(cmd.buffer, "%d", &action);
                if( ! cam->interpolating){
                    switch(action){
                        case 1:
                            if(--currentObject != objectsOrder.end()){
                                cam->setArround((*currentObject));
                            }
                            break;
                        case 2:
                            if(++currentObject != objectsOrder.end()){
                                cam->setArround((*currentObject));
                            }
                            break;
                        case 3:
                            cam->setMode(Camera::Free);
                            break;
                    }
                }

            }
           case 2: //rotation commands
                double xdelta, ydelta;
                sscanf(cmd.buffer, "%lf %lf", &xdelta, &ydelta);
                cam->vel.x = 2 * ydelta;
                cam->vel.y = 2 * xdelta;
            break;
            case 3:  // zoom
                double zdelta;
                sscanf(cmd.buffer, "%lf", &zdelta);
                cam->vel.z = -1 * zdelta;

        
        }

       delete[] cmd.buffer;
    }
    m_cmd_q.unlock();


    //Update logic
    if (Input::isKeyPressed(sf::Keyboard::P)) paused = !paused;
    if (Input::isKeyPressed(sf::Keyboard::Space)) setArroundClosestWorldObject();
    if (paused) deltaTime = 0.0f;
    if (not cam->interpolating && (Input::isKeyPressed(sf::Keyboard::Right) || SensorManager::checkGesture() == SensorManager::SWIPE_RIGHT)) {
        if (++currentObject != objectsOrder.end())
            cam->setArround((*currentObject));
        else --currentObject;
    }
    if (not cam->interpolating && (Input::isKeyPressed(sf::Keyboard::Left) || SensorManager::checkGesture() == SensorManager::SWIPE_LEFT)) {
        if (currentObject != objectsOrder.begin())
            cam->setArround((*--currentObject));
    }
    if (not cam->interpolating && (Input::isKeyPressed(sf::Keyboard::F) || SensorManager::checkGesture() == SensorManager::PUNCH)) cam->setMode(Camera::Free);
    if (not cam->interpolating && Input::isKeyPressed(sf::Keyboard::G)) cam->setMode(Camera::Arround);
    if (Input::isKeyPressed(sf::Keyboard::Num3)) {
        if (stereoscopic3D)
            glViewport(0,0,float(SCRWIDTH),float(SCRHEIGHT)); //back to normal
        stereoscopic3D = !stereoscopic3D;
    }
    if (Input::isKeyPressed(sf::Keyboard::R)) SensorManager::resetTracking();

    Input::setMousePos(SCRWIDTH/2,SCRHEIGHT/2,getGame()->getWindow());
}

void SceneSolarSystem::setArroundClosestWorldObject() {

    if (objectsOrder.begin() == objectsOrder.end()) return; // No objects on list

    std::list<std::string>::iterator it = objectsOrder.begin();
    currentObject = it;
    vec3f camPos = cam->getPosition();
    float minDist = glm::length(((WorldObject*)getGame()->getObjectByName(*currentObject))->getPosition() - camPos);
    ++it;

    for (; it != objectsOrder.end(); ++it) {
        WorldObject* wo = (WorldObject*)(getGame()->getObjectByName(*it));
        float dist = glm::length(wo->getPosition() - camPos);
        if (!(wo->id == cam->id) and dist < minDist) {
            minDist = dist;
            currentObject = it;
        }
    }

    cam->setArround(*currentObject);
}

void SceneSolarSystem::draw() const {
    if (not stereoscopic3D) {
        cam->drawHUD();
    }
    else {
        //calculate perspective matrix (Parallel)
        //getState().projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);

        //Move matrix to position (according to player/camera)
        //getState().view = mat4f(1.0);
//        std::pair<mat4f,mat4f> eyes = cam->getViewMatrix3D();

//        float ratio  = float(SCRWIDTH) / float(SCRHEIGHT);
//        float radians = DEG_TO_RAD * FOV / 2.0;
//        float wd2     = ZNEAR * glm::tan(radians);
//        //float ndfl    = ZNEAR / ((ZFAR - ZNEAR)*0.5);
//        float ndfl    = ZNEAR / 375.0;

//        float left, right, top, bottom;

//        glViewport(float(SCRWIDTH)/2.0,0,float(SCRWIDTH)/2.0,float(SCRHEIGHT)); //Right eye
//        {
//            //Off Axis projection
//            left  = - ratio * wd2 - 0.5 * cam->eyeDistance3D * ndfl;
//            right =   ratio * wd2 - 0.5 * cam->eyeDistance3D * ndfl;
//            top    =   wd2;
//            bottom = - wd2;
//            getState().projection = glm::frustum(left,right,bottom,top,ZNEAR,ZFAR);

//            getState().view = eyes.first;

//            //Drawable objects
//            glDisable(GL_CULL_FACE);
//            stars->draw();
//            glEnable(GL_CULL_FACE);

//            for(std::list<GameObject*>::const_iterator it = drawList.begin();it != drawList.end(); ++it)
//                (*it)->draw();

//            cam->drawHUD();
//        }
//        glViewport(0,0,float(SCRWIDTH)/2.0,float(SCRHEIGHT)); //Left eye
//        {
//            //Off Axis projection
//            left  = - ratio * wd2 + 0.5 * cam->eyeDistance3D * ndfl;
//            right =   ratio * wd2 + 0.5 * cam->eyeDistance3D * ndfl;
//            top    =   wd2;
//            bottom = - wd2;
//            getState().projection = glm::frustum(left,right,bottom,top,ZNEAR,ZFAR);

//            getState().view = eyes.second;

//            //Drawable objects
//            glDisable(GL_CULL_FACE);
//            stars->draw();
//            glEnable(GL_CULL_FACE);


//            //Render the shadow map
//            //m_defferedRendering->startRenderToShadowMap();
//            for(std::list<GameObject*>::const_iterator it = drawList.begin();it != drawList.end(); ++it)
//                (*it)->draw();
//            //m_defferedRendering->stopRenderToShadowMap();

//            cam->drawHUD();
//        }
    }
}

