#ifndef SCENESOLARSYSTEM_HPP
#define SCENESOLARSYSTEM_HPP

#include "Commons.hpp"
#include "Camera.hpp"
#include "SphereObject.hpp"
#include "FrameBuffer.hpp"

class Game;
class SceneSolarSystem : public GameObject {
	public:
		//virtual overrides
        SceneSolarSystem();
        ~SceneSolarSystem();

        void update(float deltaTime);
        void createTexture();
        void renderShadowMap();
        void draw() const;
        Camera* cam;

	private:
		bool loadResources();
		float debugCounter;
		int fpsCount;

        SphereObject* stars;
        FramebufferObject* pShadowFBO;
        Texture* tShadowMap;
        bool paused;
        bool stereoscopic3D;
        std::list<std::string> objectsOrder;
        std::list<std::string>::iterator currentObject;

        void setArroundClosestWorldObject();
};

#endif // SCENESOLARSYSTEM_HPP
