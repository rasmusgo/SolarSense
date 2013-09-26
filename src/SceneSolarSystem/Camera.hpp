#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Entity.hpp"
#include "../graphics/Model.hpp"

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
        void setMode(CameraMode m);

        vec2f rot;

        mat4f getViewMatrix();
        std::pair<mat4f,mat4f> getViewMatrix3D();

        CameraMode mode;

        bool interpolating;

        float eyeDistance3D; //Total distance between eyes

        static vec3f posFromMatrix(mat4f& m);
        static vec3f lerp(vec3f& a, vec3f& b, float t);

    private:        
        const float INTERPOLATION_TIME = 3.0f;

        void updateAcceleration(float deltaTime);

        mat4f rotM;
        vec3f fromPos;
        float interpolatingTimer;
        GameObject* arrObject;
        float maxAcc;
        float friction;
        float maxVel;
        sf::Clock cameraClock;
        vec3f originalPos;
        bool wasTracking;
        float handTime;
        Model hudHand;
};

#endif // CAMERA_HPP
