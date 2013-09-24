#include "SensorManager.hpp"

bool SensorManager::running(false);
bool SensorManager::tracking(false);
nite::HandTracker SensorManager::handTracker;
nite::HandId SensorManager::handId;
vec3f SensorManager::initialHandPos(-1,-1,-1);
vec3f SensorManager::lastHandPos(-1,-1,-1);
vec3f SensorManager::displacement(0,0,0);

SensorManager::SensorManager() {}

SensorManager::~SensorManager() {
    nite::NiTE::shutdown();
    openni::OpenNI::shutdown();
}

void SensorManager::startSensor() {
    /*
     * Retrieve the sensor.
     */
    // Initialize OpenNI
    openni::Status oniStatus = openni::OpenNI::initialize();
    if (oniStatus != openni::STATUS_OK) {
        printf("SensorManager | Initialization of OpenNI failed: '%s'\n", openni::OpenNI::getExtendedError());
        return;
    }

    // Retrieve an available device.
    openni::Device sensor;
    oniStatus = sensor.open(openni::ANY_DEVICE);
    if (oniStatus != openni::STATUS_OK) {
        printf("SensorManager | Failed to open the device: '%s'\n", openni::OpenNI::getExtendedError());
        openni::OpenNI::shutdown();
        return;
    }


    /*
     * Initialize NiTE which includes the gesture detection and hand tracking.
     */
    nite::Status niteStatus = nite::NiTE::initialize();
    if (niteStatus != nite::STATUS_OK) {
        printf("SensorManager | Initialization of NiTE failed.\n");
        return;
    }

    // Create the hand tracker
    niteStatus = handTracker.create();
    if (niteStatus != nite::STATUS_OK) {
        printf("SensorManager | Couldn't create the hand tracker.\n");
        return;
    }

    // Start gesture recognition for the click and wave gesture.
    handTracker.startGestureDetection(nite::GESTURE_CLICK);
    handTracker.startGestureDetection(nite::GESTURE_WAVE);


    /*
     * Intialize the GrabDetector
     */
//    grabDetector = PSLabs::CreateGrabDetector(sensor, DATA_PATH);
//    if(grabDetector == NULL || grabDetector->GetLastEvent(NULL) != openni::STATUS_OK) {
//        printf("SensorManager | Initialization of the GrabDetector failed: %d\n", grabDetector->GetLastEvent(NULL));
//        return;
//    }

//    GrabEventListener* grabListener = new GrabEventListener();
//    grabDetector->AddListener(grabListener);


    running = true;
}

void SensorManager::update() {
    // Check if the sensor is currently used
    if (!running)
        return;

    // Get next frame with hand tracking information from the sensor.
    nite::HandTrackerFrameRef frame;
    nite::Status rc = handTracker.readFrame(&frame);
    if (rc != nite::STATUS_OK)  {
        printf("SensorManager | Couldn't read frame #%d from the sensor.\n", frame.getFrameIndex());
        return;
    }

    // Get recognized gestures and the data from all tracked hands.
    const nite::Array<nite::GestureData>& gestures = frame.getGestures();
    const nite::Array<nite::HandData>& hands = frame.getHands();

    // Handle recognized gestures
    for (int i = 0; i < gestures.getSize(); ++i) {
        if (gestures[i].isComplete()) {
            switch (gestures[i].getType()) {
            case nite::GESTURE_CLICK:
//                if (!tracking)
//                    startTracking(gestures[i].getCurrentPosition());
//                else
//                    printf("SensorManager | Click gesture detected but already tracking.\n");
                break;
            case nite::GESTURE_WAVE:
                if (!tracking)
                    startTracking(gestures[i].getCurrentPosition());
                else
                    stopTracking(gestures[i].getCurrentPosition());
                break;
            case nite::GESTURE_HAND_RAISE:
                break;
            }
        }
    }

    // Update hand position and direction of hand movement.
    if (hands.getSize() > 0) { // NOTE: There should only one hand being tracked at all times.
        const nite::HandData& hand = hands[0];
        // Check if the hand could be tracked.
        if (hand.isTracking()) {
            updatePosition(hand.getPosition());
        }

        // Check if the hand was lost.
        if (hand.isLost()) {
            tracking = false;
            printf("SensorManager | Hand was lost, stopped tracking. (HandId: %d)\n", handId);
        }
    } else if (hands.getSize() > 1) {
        printf("SensorManager | Somethings went wrong: There is more than one hand being tracked.\n");
    }
}


//void SensorManager::processGrabEvent(const PSLabs::IGrabEventListener::EventParams& params) {

//}

void SensorManager::startTracking(nite::Point3f gesturePos) {
    // Save the initial hand position to determine the movement of the hand.
    initialHandPos = vec3f(gesturePos.x, gesturePos.y, gesturePos.z);

    handTracker.startHandTracking(gesturePos, &handId);

    // Tweaking the initial position.
    initialHandPos.z -100;

    printf("SensorManager | Hand detected, start tracking. (HandId: %d)\n", handId);
    tracking = true;
}

void SensorManager::stopTracking(nite::Point3f gesturePos) {
    // Calculate the distance between the position of the hand performing the gesture
    // and the last known position of the hand currently being tracked.
    vec3f temp = vec3f(gesturePos.x, gesturePos.y, gesturePos.z) - lastHandPos;
    float dist = sqrt(glm::dot(temp, temp));

    // Check if distance is sufficiently small
    if (dist <= 100) {
        handTracker.stopHandTracking(handId);
        tracking = false;
        printf("SensorManager | Stopped tracking the hand. (HandId: %d)\n", handId);

        // ISSUE: HandData::isLost() is being called after calling stopHandTracking -> two outputs.
    } else {
        printf("SensorManager | Wave gesture detected but gesture position too far away from hand position.\n");
    }

}

void SensorManager::updatePosition(nite::Point3f handPos) {
    // Update hand position
    lastHandPos = vec3f(handPos.x, handPos.y, handPos.z);

    // Calculate displacement
    displacement = lastHandPos - initialHandPos;
    displacement = vec3f(displacement.y, displacement.x, displacement.z);


//    if (abs(displacement.y) >= MOVEMENT_THRESHOLD) {
//        if (displacement.y > 0 ) {
//            printf("Moving to the right %.5f\n", displacement.y);
//        } else if (displacement.y < 0 ) {
//            printf("Moving to the left %.5f\n", displacement.y);
//        }
//    }

//    if (abs(displacement.x) >= MOVEMENT_THRESHOLD) {
//        if (displacement.x > 0 ) {
//            printf("Moving upwards: %.5f\n", displacement.x);
//        } else if (displacement.x < 0 ) {
//            printf("Moving downwards: %.5f\n", displacement.x);
//        }
//        }

//    if (abs(displacement.z) >= MOVEMENT_THRESHOLD) {
//        if (displacement.z > 0 ) {
//            printf("Moving backwards: %.5f\n", displacement.z);
//        } else if (displacement.z < 0 ) {
//            printf("Moving forwards: %.5f\n", displacement.z);
//        }
//    }
}
