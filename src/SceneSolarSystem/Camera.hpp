#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Entity.hpp"

class Camera : public Entity {
    public:
        enum CameraMode {
            Arround,
            Free
        };

        Camera(Scene* scene, const vec3f &pos);
        ~Camera();

        void draw();
        void update(float deltaTime);
        void drawHUD();

        void setArround(GameObject* object);

        vec2f rot;

        mat4f getViewMatrix();

        CameraMode mode;
    private:
        mat4f rotM;
        GameObject* arrObject;
        float maxAcc;
        float friction;
        float maxVel;
};

#endif // CAMERA_HPP
