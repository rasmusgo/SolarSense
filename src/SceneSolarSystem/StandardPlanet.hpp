#ifndef STANDARDPLANET_HPP
#define STANDARDPLANET_HPP

#include "Planet.hpp"

class StandardPlanet : public Planet {
public:
    StandardPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& texture);
    StandardPlanet(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& texture, const std::string& bumpmap);

    ~StandardPlanet();

    void update(float deltaTime);
    virtual void draw() const;
protected:
    float time;
private:
    const std::string shaderprogram;
    const std::string texturemapname;
    const std::string bumpmap;
};

#endif // STANDARDPLANET_HPP
