#include "SensorManager.hpp"


const float SensorManager::MOVEMENT_THRESHOLD = 100.f;

bool SensorManager::running(false);
bool SensorManager::trackingMovement(false);
bool SensorManager::trackingGesture(false);
openni::Device SensorManager::sensor;
nite::HandTracker SensorManager::handTracker;
nite::HandId SensorManager::movementHandId;
nite::HandId SensorManager::gestureHandId;
vec3f SensorManager::initialMovementHandPos(-1,-1,-1);
vec3f SensorManager::lastMovementHandPos(-1,-1,-1);
vec3f SensorManager::lastGestureHandPos(-1,-1,-1);
vec3f SensorManager::currentGestureHandPos(-1,-1,-1);
vec3f SensorManager::displacement(0,0,0);
vec3f SensorManager::displacementGesture(0,0,0);

SensorManager::SensorManager() {}

SensorManager::~SensorManager() {
    sensor.close();
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
                startTracking(gestures[i].getCurrentPosition());
                // Check if the wave gesture was performed by one of the hands currently being tracked.
                if (trackingMovement || trackingGesture)
                    stopTracking(gestures[i].getCurrentPosition());
                break;
            case nite::GESTURE_HAND_RAISE:
                break;
            }
        }
    }

    // Update hand position and direction of hand movement.
    if (hands.getSize() > 0) { // NOTE: There should only one hand being tracked at all times.
        const nite::HandData& hand1 = hands[0];
        const nite::HandData& hand2 = hands[1];
        nite::HandData movementHand, gestureHand;

        if (hand1.getId() == movementHandId)
            movementHand = hand1;
        else if (hand1.getId() == gestureHandId)
            gestureHand = hand1;

        if (hand2.getId() == movementHandId)
            movementHand = hand2;
        else if (hand2.getId() == gestureHandId)
            gestureHand = hand2;

        // Check if the hand could be tracked.
        if (movementHand.isTracking()) {
            updatePosition(movementHand.getPosition());
        } else if (gestureHand.isTracking()) {
            checkGesture(gestureHand.getPosition());
        }

        // Check if the hand was lost.
        if (movementHand.isLost() && trackingMovement) {
            trackingMovement = false;
            printf("SensorManager | Movement hand was lost, stopped tracking. (HandId: %d)\n", movementHandId);
        } else if (gestureHand.isLost() && trackingGesture) {
            trackingGesture = false;
            printf("SensorManager | Gesture hand was lost, stopped tracking. (HandId: %d)\n", gestureHandId);

        }
    } else if (hands.getSize() > 2) {
        printf("SensorManager | Somethings went wrong: There is more than two hand being tracked.\n");
    }
}


//void SensorManager::processGrabEvent(const PSLabs::IGrabEventListener::EventParams& params) {

//}

void SensorManager::startTracking(nite::Point3f gesturePos) {
    if (!trackingMovement) {
        handTracker.startHandTracking(gesturePos, &movementHandId);
        trackingMovement = true;
        printf("SensorManager | Hand detected, using hand for camera movement. (HandId: %d)\n", movementHandId);

        // Save the initial hand position to determine the movement of the hand.
        initialMovementHandPos = vec3f(gesturePos.x, gesturePos.y, gesturePos.z);
    } else if (!trackingGesture) {
        handTracker.startHandTracking(gesturePos, &gestureHandId);
        trackingGesture = true;
        printf("SensorManager | Hand detected, using hand gesture recognition. (HandId: %d)\n", gestureHandId);

        // Save the initial hand position to determine the movement of the hand.
        lastGestureHandPos = vec3f(gesturePos.x, gesturePos.y, gesturePos.z);
    } else {
        // Only two hands should be tracked simultaneously
        printf("SensorManager | Hand deteced but already tracking two hands, ignoring new hand.\n");
    }

}

void SensorManager::stopTracking(nite::Point3f gesturePos) {
    // Calculate the distance between the position of the hand performing the gesture
    // and the last known position of the hand currently being tracked.
    vec3f tempMov = vec3f(gesturePos.x, gesturePos.y, gesturePos.z) - lastMovementHandPos;
    vec3f tempGest = vec3f(gesturePos.x, gesturePos.y, gesturePos.z) - lastMovementHandPos;
    float distMov = sqrt(glm::dot(tempMov, tempMov));
    float distGest = sqrt(glm::dot(tempGest, tempGest));

    // Check if distance is sufficiently small
    if (distMov <= 100) {
        handTracker.stopHandTracking(movementHandId);
        trackingMovement = false;
        printf("SensorManager | Stopped tracking the movement hand. (HandId: %d)\n", movementHandId);
    } else if (distGest <= 100) {
        handTracker.stopHandTracking(gestureHandId);
        printf("SensorManager | Stopped tracking the gesturing hand. (HandId: %d)\n", gestureHandId);

    } else {
        printf("SensorManager | Wave gesture detected but gesture position too far away from any hand position.\n");
    }
}

void SensorManager::updatePosition(nite::Point3f handPos) {
    // Update hand position
    lastMovementHandPos = vec3f(handPos.x, handPos.y, handPos.z);

    // Calculate displacement
    displacement = lastMovementHandPos - initialMovementHandPos;
    displacement = vec3f(displacement.y, displacement.x, displacement.z);
}

int SensorManager::gestureDetected() {
    if (displacementGesture.x >= 150) {
        return 1;
    } else if (displacementGesture.x <= -150) {
        return -1;
    }

    return 0;
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

void SensorManager::checkGesture(nite::Point3f handPos) {
    currentGestureHandPos = vec3f(handPos.x, handPos.y, handPos.z);
    displacementGesture = currentGestureHandPos - lastGestureHandPos;
    lastGestureHandPos = currentGestureHandPos;

}
