#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "tools.hpp"

class Scene;
class GameObject {
	public:
		GameObject(Scene* scene, const vec3f &pos, const vec3f &scale);
		virtual ~GameObject();

		virtual void update(float deltaTime);
		virtual void draw() const;
		virtual void addObject(GameObject* object);

        virtual mat4f getModelMatrix();

		bool isAlive;
		vec3f pos;
		vec3f scale;
	protected:
		Scene* parentScene;
		std::list<GameObject*> objects;
};

#endif // GAMEOBJECT_HPP
