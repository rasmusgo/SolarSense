#ifndef SUN_HPP
#define SUN_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"
#include "Planet.hpp"

class Sun : public Planet {
    public:
        Sun(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed);
        ~Sun();

        void update(float deltaTime);
        void draw() const;
        void drawFrom(mat4f from) const;
        mat4f getModelMatrix();

    GameObject* parentObject;
};

#endif // SUN_HPP
