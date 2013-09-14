#ifndef SCENESOLARSYSTEM_HPP
#define SCENESOLARSYSTEM_HPP
#include "../Scene.hpp"
#include "Camera.hpp"

class GameObject;
class ShaderProgram;
class SceneSolarSystem : public Scene {
	public:
		//virtual overrides
        SceneSolarSystem(SolarSenseApp &parent);
        ~SceneSolarSystem();

		void update(float deltaTime);
		void draw() const;

        Camera* cam;

	private:
		bool loadResources();
		float debugCounter;
		int fpsCount;
};

#endif // SCENESOLARSYSTEM_HPP
