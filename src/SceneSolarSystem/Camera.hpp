#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Commons.hpp"
#include "WorldObject.hpp"

class Camera : public WorldObject {
    public:
        enum CameraMode {
            Arround,
            Free
        };

        Camera(const vec3f& pos = vec3f(0.0f), const mat4f& projection = mat4f(1.0f));
        ~Camera();

        void draw();
        void update(float deltaTime, float time);
        void drawHUD();

        mat4f projection;
        mat4f view;

        //
        vec3f vel;
        vec3f acc;

        void setArround(const std::string& objectName);
        void setArround(WorldObject* object);
        void setMode(CameraMode m);

        std::pair<mat4f,mat4f> getViewMatrix3D();

        CameraMode mode;

        bool interpolating;

        float eyeDistance3D; //Total distance between eyes

        mat4f billboard (vec3f position);

        void setStereoscopic(bool s3d);
        void setEye(int i);

        int currEye;

    private:        
        const float INTERPOLATION_TIME = 3.0f;

        void updateAcceleration(float deltaTime);

        vec3f fromPos;
        float interpolatingTimer;
        quat initialRotationInterpolationQuat;
        WorldObject* arrObject;
        float lastArrDist;
        float maxAcc;
        float friction;
        float maxVel;
        sf::Clock cameraClock;
        vec3f originalPos;
        bool wasTracking;
        float handTime;
        Model hudHand;
        float focus;
        float focusSpeed;

        bool stereoscopic3D;
        std::pair<mat4f,mat4f> eyes;
};

#endif // CAMERA_HPP
