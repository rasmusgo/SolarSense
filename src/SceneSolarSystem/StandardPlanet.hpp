#ifndef STANDARDPLANET_HPP
#define STANDARDPLANET_HPP

#include "../GameObject.hpp"
#include "../graphics/Model.hpp"
#include "Planet.hpp"

class StandardPlanet : public Planet {
    public:
        StandardPlanet(Scene* parentScene, GameObject* parentObject, vec3f scale, float orbRadius, float orbSpeed, const char* shaderprogram, const char* texture);
        ~StandardPlanet();

        void update(float deltaTime);
        void draw() const;
        void drawFrom(mat4f from) const;

   	GameObject* parentObject;
   private:
   	const char* shaderprogram;
   	const char* texturemapname;
   	
};

#endif // STANDARDPLANET_HPP
