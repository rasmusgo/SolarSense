#ifndef SUNHALO_HPP
#define SUNHALO_HPP

#include "Commons.hpp"
#include "Planet.hpp"

class SunHalo : public GameObject {
    public:
        SunHalo();
        ~SunHalo();

        void update(float deltaTime);
        void draw() const;

        float scale;

        Model model;
    private:
        float time;
};

#endif // SUN_HPP
