#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Commons.hpp"

class Camera : public GameObject {
    public:
        enum CameraMode {
            Arround,
            Free
        };

        Camera(const vec3f& pos = vec3f(0.0f), const vec3f& rot = vec3f(0.0f), const mat4f& projection = mat4f(1.0f));
        ~Camera();

        void draw();
        void update(float deltaTime);
        void drawHUD();

        vec3f pos;
        vec3f rot;
        mat4f projection;
        mat4f view;

        //
        vec3f vel;
        vec3f acc;

        void setArround(GameObject* object);
        void setMode(CameraMode m);

        std::pair<mat4f,mat4f> getViewMatrix3D();

        CameraMode mode;

        bool interpolating;

        float eyeDistance3D; //Total distance between eyes

    private:        
        const float INTERPOLATION_TIME = 3.0f;

        void updateAcceleration(float deltaTime);

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
