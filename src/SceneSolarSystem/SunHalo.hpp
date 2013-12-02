#ifndef SUNHALO_HPP
#define SUNHALO_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"

class SunHalo : public WorldObject {
    public:
        SunHalo(const std::string& name, float size);
        ~SunHalo();

        void update(float deltaTime, float time);
        void draw() const;

        Model model;
    private:
        float time;
};

#endif // SUN_HPP
