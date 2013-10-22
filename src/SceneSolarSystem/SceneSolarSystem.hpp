#ifndef SCENESOLARSYSTEM_HPP
#define SCENESOLARSYSTEM_HPP

#include "Commons.hpp"

class Game;
class SceneSolarSystem : public GameObject {
	public:
		//virtual overrides
        SceneSolarSystem(SolarSenseApp &parent);
        ~SceneSolarSystem();

        void update(float deltaTime);

        Camera* cam;

	private:
		bool loadResources();
		float debugCounter;
		int fpsCount;

        GameObject* stars;
        bool paused;
        bool stereoscopic3D;
        std::list<std::string> objectsOrder;
        std::list<std::string>::iterator currentObject;
};

#endif // SCENESOLARSYSTEM_HPP
