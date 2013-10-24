#ifndef SUNHALO_HPP
#define SUNHALO_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"

class SunHalo : public WorldObject {
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
