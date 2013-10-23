#ifndef EARTH_HPP
#define EARTH_HPP

#include "Planet.hpp"

class Earth : public Planet {
    public:
        Earth(const std::string& name);
        ~Earth();

        void update(float deltaTime);
        void draw() const;
    private:
        float time;
};

#endif // EARTH_HPP
