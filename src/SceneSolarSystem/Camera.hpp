#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Entity.hpp"

class Camera : public Entity {
    public:
        Camera(Scene* scene, const vec3f &pos);
        ~Camera();

        void draw();
        void update(float deltaTime);
        void drawHUD();

        vec2f rot;

        mat4f getViewMatrix();
    private:
        mat4f rotM;
};

#endif // CAMERA_HPP
