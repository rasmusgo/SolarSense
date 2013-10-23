#ifndef STANDARDPLANET_HPP
#define STANDARDPLANET_HPP

#include "Planet.hpp"

class StandardPlanet : public Planet {
public:
    StandardPlanet(const std::string& name, const std::string& shaderprogram, const std::string& texture);
    StandardPlanet(const std::string& name, const std::string& shaderprogram, const std::string& texture, const std::string& bumpmap);

    ~StandardPlanet();

    void update(float deltaTime);
    void draw() const;
private:
    const std::string shaderprogram;
    const std::string texturemapname;
    const std::string bumpmap;
    float time;
};

#endif // STANDARDPLANET_HPP
