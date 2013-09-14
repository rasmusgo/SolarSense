#ifndef SCENE_HPP
#define SCENE_HPP
#include "tools.hpp"
#include "GameObject.hpp"

struct RenderState {
		RenderState() : model(1.0f), view(1.0f), projection(1.0f) {}
		~RenderState() {}
		mat4f model,view,projection;
};

class SolarSenseApp;
class Scene : public GameObject{ //abstract
	public:
        Scene(SolarSenseApp &parent);
		virtual ~Scene();

		virtual void update(float deltaTime) = 0;
		virtual void draw() const = 0;

		RenderState &getState() const;
	protected:
		virtual bool loadResources() = 0;

        SolarSenseApp& parent;
		mutable RenderState state;
};

#endif // SCENE_HPP
