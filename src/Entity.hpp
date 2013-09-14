#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "GameObject.hpp"

class Entity : public GameObject { // Moving GameObject (with velocity and acceleration)

    public:
        Entity(Scene* scene, const vec3f &pos, const vec3f &scale);
        virtual ~Entity();

        virtual void draw() const;
        virtual void update(float deltaTime);

        vec3f vel; // velocity
        vec3f acc; // acceleration
};

#endif // ENTITY_HPP
