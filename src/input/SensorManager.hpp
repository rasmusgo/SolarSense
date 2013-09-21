#ifndef SENSORMANAGER_HPP
#define SENSORMANAGER_HPP

#define MOVEMENT_THRESHOLD 100

#include "tools.hpp"
#include "NiTE.h"

using namespace nite;

class SensorManager {
    public:
        /**
          * Intializes the connected Sensor (PrimeSense, Kinect) and starts the gesture recognition.
          */
       static void startSensor();
       /**
         * Retrieves sensor data and processes it.
         * Should be called as often as possible.
         */
       static void update();

       /**
         * Checks if there is currently a hand being tracked.
         *
         * @return True if a hand is tracked, false if not.
         */
       static bool isTracking() {return tracking;}
       /**
         * Call this method to check if there is significant movement.
         *
         * @return True if there is significant movement, false if not.
         */
       static bool significantMovement() {return abs(displacement.x) >= MOVEMENT_THRESHOLD ||
                                                 abs(displacement.y) >= MOVEMENT_THRESHOLD ||
                                                 abs(displacement.z) >= MOVEMENT_THRESHOLD;}
       /**
         * Returns a vector indicating the movement of the tracked hand.
         * The first component indicates the movement in the x (left, right) direction.
         * The second component indicates the movement in the y (down, up) direction.
         * The third component indicates the movement in the z (forward, backward) direction.
         *
         * @return The displacement of the tracked hand to its initial position.
         */
       static vec3f getHandMovement() {return displacement;}

    private:
        SensorManager();
        ~SensorManager();

        static void startTracking(Point3f gesturePos);
        static void stopTracking(Point3f gesturePos);
        static void updatePosition(Point3f handPos);

        static bool running;
        static bool tracking;
        static HandTracker handTracker;
        static HandId handId;
        static vec3f initialHandPos, lastHandPos, displacement;
        static int framesSinceLastMovement;
};

#endif // SENSORMANAGER_HPP
