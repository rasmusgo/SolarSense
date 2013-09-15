#ifndef PLANET_HPP
#define PLANET_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"

class Planet : public GameObject {
    public:
        Planet(Scene* parentScene, GameObject* parentObject);
        ~Planet();

        virtual void update(float deltaTime);
        virtual void draw() const;
        virtual void drawFrom(mat4f from) const;
        virtual void addObject(GameObject* object);

    private:
        float orbSpeed;
        float orbRadius;
        Model sphere;//should be probably an abstract class so this would go out
        mat4f baseMatrix;
        std::list<GameObject*> orbObjects;

};

#endif // PLANET_HPP
