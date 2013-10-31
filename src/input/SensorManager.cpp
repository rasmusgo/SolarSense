#include "SensorManager.hpp"


const float SensorManager::MOVEMENT_THRESHOLD = 100.f;
const float SensorManager::TIME_THRESHOLD = 0.5f;
const float SensorManager::GESTURE_VELOCITY_THRESHOLD = 50.f;
const float SensorManager::GESTURE_DISTANCE_THRESHOLD = 30.f;


bool SensorManager::running(false);
bool SensorManager::tracking(false);
bool SensorManager::gestureInProgress(false);
float SensorManager::detectTime(0.f);

openni::Device SensorManager::sensor;
nite::HandTracker SensorManager::handTracker;
nite::HandId SensorManager::handId;

vec3f SensorManager::initialHandPos(-1,-1,-1);
vec3f SensorManager::lastHandPos(-1,-1,-1);
vec3f SensorManager::gestureStartPosition(0,0,0);
vec3f SensorManager::displacement(0,0,0);
vec3f SensorManager::velocity(0,0,0);
vec3f SensorManager::gestureDistance(0,0,0);


SensorManager::SensorManager() {}

SensorManager::~SensorManager() {
    sensor.close();
    openni::OpenNI::shutdown();
    nite::NiTE::shutdown();
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
        sensor.close();
        openni::OpenNI::shutdown();
        printf("SensorManager | Initialization of NiTE failed.\n");
        return;
    }

    // Create the hand tracker
    niteStatus = handTracker.create();
    if (niteStatus != nite::STATUS_OK) {
        sensor.close();
        openni::OpenNI::shutdown();
        nite::NiTE::shutdown();
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

    printf("SensorManager running\n");
    running = true;
}

void SensorManager::update(float deltaTime) {
    // Check if the sensor is currently used
    if (!running)
        return;

    // Get next frame with hand tracking information from the sensor.
    nite::HandTrackerFrameRef frame;
    nite::Status rc = handTracker.readFrame(&frame);
    if (rc != nite::STATUS_OK)  {
        printf("SensorManager | ERROR: Couldn't read frame #%d from the sensor.\n", frame.getFrameIndex());
        return;
    }

    // Get recognized gestures and the data from all tracked hands.
    const nite::Array<nite::GestureData>& gestures = frame.getGestures();
    const nite::Array<nite::HandData>& hands = frame.getHands();

    // Handle recognized gestures
    for (int i = 0; i < gestures.getSize(); ++i) {
        if (gestures[i].isInProgress()) {
            printf("Gesture %d in progress...\n", gestures[i].getType());
        }

        if (gestures[i].isComplete()) {
            switch (gestures[i].getType()) {
            case nite::GESTURE_CLICK:
                break;
            case nite::GESTURE_WAVE:
                if (!tracking) {
                    startTracking(gestures[i].getCurrentPosition());
                    detectTime = deltaTime;
                } else
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
        if (hand.getId() == handId && hand.isTracking())
            updateHandData(hand.getPosition());

        // Update time
        detectTime += deltaTime;

        // Check if the hand was lost.
        if (hand.isLost())
            resetTracking();
    } else if (hands.getSize() > 1) {
        printf("SensorManager | WARNING: There is more than two hand being tracked.\n");
    }

}

void SensorManager::resetTracking() {
    handTracker.stopHandTracking(handId);
    tracking = false;
    detectTime = 0.f;
    velocity = vec3f(0,0,0);

    std::string s;
    if (stopTracking)
        s = "SensorManager | Stopped tracking of hand %d.\n";
    else
        s = "SensorManager | Hand %d lost.\n";
    printf(s.c_str(), handId);
}


//void SensorManager::processGrabEvent(const PSLabs::IGrabEventListener::EventParams& params) {

//}

void SensorManager::startTracking(nite::Point3f gesturePos) {
    handTracker.startHandTracking(gesturePos, &handId);
    tracking = true;
    printf("SensorManager | Hand detected, Id %d\n", handId);

    // Save the initial hand position to determine the movement of the hand.
    initialHandPos = vec3f(gesturePos.x, gesturePos.y, gesturePos.z);
}

void SensorManager::stopTracking(nite::Point3f gesturePos) {
    // Calculate the distance between the position of the hand performing the gesture
    // and the last known position of the hand currently being tracked.
    vec3f temp = vec3f(gesturePos.x, gesturePos.y, gesturePos.z) - lastHandPos;
    float distance = sqrt(glm::dot(temp, temp));

    // Check if distance is sufficiently small
    if (distance <= 100) {
        resetTracking();
    } else {
        printf("SensorManager | WARNING: Wave gesture detected but gesture position too far away from any hand position.\n");
    }
}

void SensorManager::updateHandData(nite::Point3f handPos) {
    // Update hand position
    vec3f currentHandPos = vec3f(handPos.x, handPos.y, handPos.z);

    // Calculate displacement
    displacement = currentHandPos - initialHandPos;
    velocity = currentHandPos - lastHandPos;

    // Swap x and y coordinates for the camera
    displacement = vec3f(displacement.y, displacement.x, displacement.z);
    velocity = vec3f(velocity.y, velocity.x, velocity.z);

    lastHandPos = currentHandPos;

}

int SensorManager::checkGesture() {
    // Do not recognize gestures when the tracking has just begun.
    if (detectTime >= TIME_THRESHOLD) {
        if (!gestureInProgress) {
            gestureStartPosition = lastHandPos;
            gestureDistance = vec3f(0);
        } else {
            // Update distance the hand moved.
            gestureDistance = glm::abs(lastHandPos - gestureStartPosition);
        }

        // Check if the horizontal velocity is high enough
        if (velocity.y >= GESTURE_VELOCITY_THRESHOLD) {
            gestureInProgress = true;

            // Check if the distance the hand moved with the velocity is high enough.
            if (gestureDistance.y >= GESTURE_DISTANCE_THRESHOLD)
                return SWIPE_RIGHT;
        } else if (velocity.y <= -GESTURE_VELOCITY_THRESHOLD) {
            gestureInProgress = true;

            // Check if the distance the hand moved with the velocity is high enough.
            if (gestureDistance.y >= GESTURE_DISTANCE_THRESHOLD)
                return SWIPE_LEFT;
        } else {
            gestureInProgress = false;
        }

        if (velocity.z <= -125) {
            //gestureInProgress = true;

            //if (gestureDistance.z >= GESTURE_DISTANCE_THRESHOLD)
                return PUNCH;
        } else {
            //gestureInProgress = false;
        }
    }

    return NO_GESTURE;
}

vec3f SensorManager::getHandMovement() {
    vec3f d = displacement;
    float max = MOVEMENT_THRESHOLD*2.5;
    float min = MOVEMENT_THRESHOLD;

    if (abs(displacement.x) >= min) {
        if (displacement.x > 0 )
            d.x = glm::min(d.x - min, max);
        else if (displacement.x < 0 )
            d.x = glm::max(d.x + min, -max);
    }
    else d.x = 0.0f;

    if (abs(displacement.y) >= min) {
        if (displacement.y > 0 )
            d.y = glm::min(d.y - min, max);
        else if (displacement.y < 0 )
            d.y = glm::max(d.y + min, -max);
    }
    else d.y = 0.0f;

    if (abs(displacement.z) >= min) {
        if (displacement.z > 0 )
            d.z = glm::min(d.z - min, max);
        else if (displacement.y < 0 )
            d.z = glm::max(d.z + min, -max);
    }
    else d.z = 0.0f;

    return d / (max-min);
}
