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

        void addObject(GameObject* obj);
        void addObject(const std::string& name, GameObject* obj);
        void addDrawableObject(GameObject* dObj);
        void addDrawableObject(const std::string& name, GameObject* dObj);

        Camera* cam;

	private:
        GameObject* stars;

		bool loadResources();
		float debugCounter;
		int fpsCount;
        std::list<GameObject*> drawList;
        bool paused;
        bool stereoscopic3D;
        std::map<std::string, GameObject*> objectsMap;
        std::list<std::string> objectsOrder;
        std::list<std::string>::iterator currentObject;
};

#endif // SCENESOLARSYSTEM_HPP
