#ifndef ORBIT_HPP
#define ORBIT_HPP

#include "Commons.hpp"

// Just can be son of Planets

class Orbit : public GameObject {
public:
    Orbit();

    ~Orbit();

    void update(float deltaTime, float time);
    void draw() const;

private:
    Model orbit;
};

#endif // ORBIT_HPP
