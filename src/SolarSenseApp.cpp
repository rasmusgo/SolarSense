#include "SolarSenseApp.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

SolarSenseApp::SolarSenseApp() : currentScene(NULL), nextScene(NULL) {
    window.create(sf::VideoMode(SCRWIDTH,SCRHEIGHT,32), WINDOW_TITLE ,sf::Style::Fullscreen,CONTEXT_SETTINGS_OPENGL);
        window.setMouseCursorVisible(false);
        window.setKeyRepeatEnabled(false);
        window.setVerticalSyncEnabled(true);
        WINDOWFOCUS = true;
    glClearColor(0.0/255.0,0.0/255.0,0.0/255.0,1);

    std::cout << "OpenGL version: " << window.getSettings().majorVersion << std::endl;
    std::cout << "Anti aliasing level" << std::endl << window.getSettings().antialiasingLevel << std::endl;
}

SolarSenseApp::~SolarSenseApp() {
	if (currentScene != NULL) {
		delete currentScene;
		currentScene = NULL;
	}
	std::cout << "* EXITING GAME" << std::endl;
	window.close();
}

// Init non-resource, general game frame stuff here.
bool SolarSenseApp::init() {
	std::cout << "* INIT GAME" << std::endl;

	//Load game-wide resources
	if (!loadResources())
		return false;
	isRunning = true;

    // Initialize sensor
    SensorManager::startSensor();

	
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

	//initialise game-wide logic and objects
	std::cout << "* INIT GAME SUCCESFUL" << std::endl;
	return true;
}

// Load scene-independent resources here, return false if failed to load
bool SolarSenseApp::loadResources () {
	return true;
}

// Main game loop
void SolarSenseApp::run() {
    sf::Clock clock;

    while (isRunning) {
        float deltaTime = clock.restart().asSeconds();

		update(deltaTime);
        window.setActive();
        draw();
	}
}

// 1: Change scene if necessary
// 2: Update game-wide logic
// 3: Process input
// 4: Update scene
void SolarSenseApp::update(float deltaTime) {

	//Change scene, initialize it and close if it fails to initialize
	if (nextScene != NULL) {
		if (currentScene != NULL)
			delete currentScene;
		currentScene = nextScene;
		nextScene = NULL;
	}

	//Check window events. Events handled by main game object (scene-independent):
	// - Closing window
	// - Resizing window & viewport
	// - Updating window focus
    KeyAndMouseManager::update(isRunning,window);
    SensorManager::update(deltaTime);

	//Scene logic updating
	if (currentScene != NULL)
		currentScene->update(deltaTime);
}

// Draw scene
void SolarSenseApp::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentScene != NULL)
        currentScene->draw();


    sf::Font font;
    font.loadFromFile("data/arial.ttf");
    sf::Text text("No hand detected", font);
    if (SensorManager::isTracking()) {
        text.setString("Tracking hand");
    } else {
        text.setString("No hand detected");
    }
    text.setPosition(50, 50);

    // sf::Texture texture;
    // texture.loadFromFile("data/cubetex.png");
    // sf::Sprite sprite; // sprite.setTexture(texture);

//    glUseProgram(0);
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glDisable(GL_CULL_FACE);
//    window.pushGLStates();
//    window.draw(text);
//    window.popGLStates();
//    glEnable(GL_CULL_FACE);

    window.display();
}

// Change scene so that on next this->update(), this->currentScene will be replaced
void SolarSenseApp::setScene (Scene * scene) {
	if(nextScene != NULL)
		delete nextScene;
	nextScene = scene;
}

void SolarSenseApp::drawText(sf::Text &t) {
    // Before drawing text
    glMatrixMode(GL_MODELVIEW); glPushMatrix();
    glMatrixMode(GL_PROJECTION); glPushMatrix();
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT   | GL_ENABLE_BIT  |
                GL_TEXTURE_BIT      | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

        // Draw the text
        window.draw(t);

    // After drawing the text
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
    glPopAttrib();
}
