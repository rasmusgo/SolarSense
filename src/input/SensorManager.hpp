#ifndef SENSORMANAGER_HPP
#define SENSORMANAGER_HPP

#include "tools.hpp"
#include "OpenNI.h"
#include "NiTE.h"

//#include "GrabDetector.h"

class SensorManager {
    public:
        // Singleton method
        static SensorManager& getInstance() {
            static SensorManager instance;
            return instance;
        }

        static const float MOVEMENT_THRESHOLD;
        static const float TIME_THRESHOLD;
        static const float GESTURE_VELOCITY_THRESHOLD;
        static const float GESTURE_DISTANCE_THRESHOLD;

        /**
          * Intializes the connected Sensor (PrimeSense, Kinect) and starts the gesture recognition.
          */ 
       static void startSensor();

       /**
         * Retrieves sensor data and processes it.
         * Should be called as often as possible.
         */
       static void update(float deltaTime);

       static void resetInitialHandPos() {initialHandPos = lastHandPos; velocity = vec3f(0,0,0);}

       static void resetTracking();

       /**
         * Checks there is a sensor connected to the pc.
         *
         * @return True if a sensor is connected, false if not.
         */
       static bool sensorConnected() {return running;}

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
       static bool significantMovement() {
           return abs(displacement.x) >= MOVEMENT_THRESHOLD ||
                  abs(displacement.y) >= MOVEMENT_THRESHOLD ||
                  abs(displacement.z) >= MOVEMENT_THRESHOLD;
       }

       /**
         * Returns a vector indicating the movement of the tracked hand.
         * The first component indicates the movement in the x (left, right) direction.
         * The second component indicates the movement in the y (down, up) direction.
         * The third component indicates the movement in the z (forward, backward) direction.
         *
         * @return The displacement of the tracked hand to its initial position.
         */
       static vec3f getHandMovement();

       static int checkGesture();

       enum GestureType {
           NO_GESTURE,
           SWIPE_LEFT,
           SWIPE_RIGHT,
           PUNCH
       };

//       static void processGrabEvent(const PSLabs::IGrabEventListener::EventParams& params);

//       // Inner class for the GrabListener
//       class GrabEventListener : public PSLabs::IGrabEventListener {
//           public:
//               virtual void DLL_CALL ProcessGrabEvent(const EventParams& params) {
//                   SensorManager::processGrabEvent(params);
//               }
//       };

    private:
        SensorManager();
        SensorManager(SensorManager const&);
        void operator=(SensorManager const&);
        ~SensorManager();

        static void startTracking(nite::Point3f gesturePos);
        static void stopTracking(nite::Point3f gesturePos);
        static void updateHandData(nite::Point3f handPos);

        static bool running, tracking, gestureInProgress;
        static float detectTime;

        static openni::Device sensor;

        static nite::HandTracker handTracker;
        static nite::HandId handId;
        //static PSLabs::IGrabDetector* grabDetector;

        static vec3f initialHandPos, lastHandPos, gestureStartPosition, displacement, velocity, gestureDistance;
};

#endif // SENSORMANAGER_HPP
