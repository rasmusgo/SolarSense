#ifndef SOLARSENSEAPP_HPP
#define SOLARSENSEAPP_HPP

/*
 * Code structure and basic classes taken from Towerthousand's WIP game engine at https://github.com/Towerthousand/VBE
 */


#include "graphics/TextureManager.hpp"
#include "graphics/MeshManager.hpp"
#include "graphics/ShaderManager.hpp"
#include "audio/AudioManager.hpp"
#include "input/KeyAndMouseManager.hpp"
#include "input/SensorManager.hpp"
#include "Scene.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Uniform.hpp"

class SolarSenseApp {
	public:
        SolarSenseApp(); //creates window
        ~SolarSenseApp();
        bool init(); // inits app-wide stuff here (init and resource loading)
        void run(); // won't run if SolarSenseAppe::init() isn't called first
		// contains main loop, calls update() and draw()
		void setScene(Scene * scene); // sets nextScene to scene, which will
		// move into currentScene on next update()
		// so that there is no update() of one
		// scene followed by a draw() method
		// of a different scene.
		void close(); // closes app completely, closing the current scene (if there is)
		// first, saves game-wide stuff first.

		sf::RenderWindow &getWindow() { return window; }

        void drawText(sf::Text& t);
		
		bool isRunning;
	private:
		void update(float deltaTime); // changes scene if necessary
		// updates fps
		// checks for window events
        // updates input
		// calls currentScene.update(deltaTime)
		void draw(); // calls currentScene.draw()
        bool loadResources (); // loads app-wide resources. only called
		// by init() once

		//context
    	sf::ContextSettings settings;
		sf::RenderWindow window;
		Scene* currentScene;
		Scene* nextScene;
};

#endif //SOLARSENSEAPP_HPP
