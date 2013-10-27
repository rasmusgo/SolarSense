#ifndef SCENESOLARSYSTEM_HPP
#define SCENESOLARSYSTEM_HPP

#include "Commons.hpp"
#include "Camera.hpp"
#include "SphereObject.hpp"

class Game;
class SceneSolarSystem : public GameObject {
	public:
		//virtual overrides
        SceneSolarSystem();
        ~SceneSolarSystem();

        void update(float deltaTime);
        void draw() const;

        Camera* cam;

	private:
		bool loadResources();
		float debugCounter;
		int fpsCount;

        SphereObject* stars;
        bool paused;
        bool stereoscopic3D;
        std::list<std::string> objectsOrder;
        std::list<std::string>::iterator currentObject;

        std::pair<WorldObject*,bool> closestWorldObject();
        void setCameraArround(WorldObject* o);
};

#endif // SCENESOLARSYSTEM_HPP
