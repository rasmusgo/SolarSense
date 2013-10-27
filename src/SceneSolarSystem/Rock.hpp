#ifndef ROCK_HPP
#define ROCK_HPP

#include "Planet.hpp"

class Rock : public Planet {
public:
    Rock(const std::string& name, float radius, float orbRadius, const std::string& shaderprogram, const std::string& texture);

    ~Rock();

    void update(float deltaTime);
    void draw() const;
private:
    const std::string shaderprogram;
    const std::string texturemapname;
    const std::string bumpmap;
    float time;
};

#endif // ROCK_HPP
