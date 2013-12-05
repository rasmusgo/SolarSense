#include "PrimeSense.h"
#include <math.h>

PrimeSense::PrimeSense()
:	trackingUser(false), userReady(false), userLost(false), startedTrackingUser(false), stoppedTrackingUser(false), grabbing(false),
	width(0), height(0),
	lastVisibleTime(0), startedTrackingTime(0), gesture(NONE),
	trackingUserId(-1),
	depthFrame(openni::VideoFrameRef()), colorFrame(openni::VideoFrameRef()),
	handTracker(nite::HandTracker()),
	grabDetector(NULL), grabListener(NULL) {

	// Restart the clock
	clock.restart();

	openni::Status rc = openni::STATUS_OK;
	const char* deviceURI = openni::ANY_DEVICE;

	rc = openni::OpenNI::initialize();
	if (rc != openni::STATUS_OK) {
		printf("PrimeSense | OpenNI Initialization failed:\n%s\n", openni::OpenNI::getExtendedError());
		return;
	}

	if (nite::NiTE::initialize() != nite::STATUS_OK) {
		printf("PrimeSense | NiTE2 Initialization failed!");
		return;
	}

	rc = device.open(deviceURI);
	if (rc != openni::STATUS_OK) {
		printf("PrimeSense | Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
		openni::OpenNI::shutdown();
		return;
	}

	rc = device.setDepthColorSyncEnabled(true);

	openni::VideoMode videoMode;
	int resX = 640, resY = 480, fps = 30;

	rc = depthStream.create(device, openni::SENSOR_DEPTH);
	if (rc == openni::STATUS_OK) {
		videoMode = depthStream.getVideoMode();
		videoMode.setFps(fps);
		videoMode.setResolution(resX, resY);
		rc = depthStream.setVideoMode(videoMode);
		if (rc != openni::STATUS_OK) {
			printf("PrimeSense | Couldn't set depth mode:\n%s\n", openni::OpenNI::getExtendedError());
			return;
		}

		rc = depthStream.start();
		if (rc != openni::STATUS_OK) {
			printf("PrimeSense | Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			depthStream.destroy();
			return;
		}
	} else {
		printf("PrimeSense | Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
		return;
	}

	rc = colorStream.create(device, openni::SENSOR_COLOR);
	if (rc == openni::STATUS_OK) {
		videoMode = colorStream.getVideoMode();
		videoMode.setFps(fps);
		videoMode.setResolution(resX, resY);
		rc = colorStream.setVideoMode(videoMode);
		if (rc != openni::STATUS_OK) {
			printf("PrimeSense | Couldn't set color mode:\n%s\n", openni::OpenNI::getExtendedError());
			return;
		}

		rc = colorStream.start();
		if (rc != openni::STATUS_OK) {
			printf("PrimeSense | Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
			colorStream.destroy();
			return;
		}
	} else {
		printf("PrimeSense | Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
		return;
	}

	//Set stream parameters
	depthStream.setMirroringEnabled(true);
	colorStream.setMirroringEnabled(true);

	//Set registration. This is very important as we cannot use image stream if they are not registered
	rc = device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
	if (rc != openni::STATUS_OK) {
		printf("PrimeSense | Couldn't set image to depth registration. Disabling image stream.\n%s\n", openni::OpenNI::getExtendedError());
		colorStream.stop();
		colorStream.destroy();
	}

	if (!depthStream.isValid()) {
		printf("PrimeSense | No valid depth stream. Exiting\n");
		openni::OpenNI::shutdown();
		return;
	}

	std::ifstream file("Data/grab_gesture.dat");
	if (!file) {
		printf("PrimeSense | Cannot find \"Data/grab_gesture.dat\"");
		openni::OpenNI::shutdown();
		return;
	}
	file.close();

	rc = init();
	if (rc != openni::STATUS_OK) {
		openni::OpenNI::shutdown();
		return ;
	}
}

PrimeSense::~PrimeSense() {}

//This function updates the algorithms after a new frame has been read
void PrimeSense::update()
{
	float dt = clock.getElapsedTime().asSeconds();

	depthStream.readFrame(&depthFrame);
	if (colorStream.isValid())
		colorStream.readFrame(&colorFrame);

	userLost = false;
	userLost = false;

	//Update NiTE trackers and get their result
	updateUserTracker(dt);

	// Update grab detector
	if (grabDetector != NULL) {
		//If the user is lost, we need to reset the grab detector
		if (userLost) {
			grabDetector->Reset();
			//If a user is being tracked and the skeleton is ready
		} else if (trackingUser && userReady) {
			nite::Point3f handPos = userInfo.rightHandPos;
			grabDetector->SetHandPosition(handPos.x, handPos.y, handPos.z);
		}

		//Update algorithm with the newly read frames. We prefer both frames, but can work only with one
		if (depthFrame.isValid() && colorFrame.isValid())
			grabDetector->UpdateFrame(depthFrame, colorFrame);
		else if (depthFrame.isValid())
			grabDetector->UpdateFrame(depthFrame);
	}

	// Check gestures
	if (!grabbing && trackingUser) {
		float dt = clock.getElapsedTime().asSeconds();
		checkGesture(dt);
	} else {
		//distance = 0;
	}
}

void PrimeSense::getHandPosition(float* handX, float* handY, float* handZ) {
	nite::Point3f handPos = userInfo.rightHandPos;
	(*handX) = handPos.x;
	(*handY) = handPos.y;
	(*handZ) = handPos.z;
}


bool PrimeSense::startedTracking() {
	if (startedTrackingUser) {
		startedTrackingUser = false;
		return true;
	}

	return false;
}

bool PrimeSense::stoppedTracking() {
	if (stoppedTrackingUser) {
		stoppedTrackingUser = false;
		return true;
	}

	return false;
}

// Initialize depth and color stream from the PrimeSense, as well as the hand tracker and the grab detector
openni::Status PrimeSense::init() {
	openni::VideoMode depthVideoMode;
	openni::VideoMode colorVideoMode;

	if (depthStream.isValid() && colorStream.isValid())	{
		depthVideoMode = depthStream.getVideoMode();
		colorVideoMode = colorStream.getVideoMode();

		int depthWidth = depthVideoMode.getResolutionX();
		int depthHeight = depthVideoMode.getResolutionY();
		int colorWidth = colorVideoMode.getResolutionX();
		int colorHeight = colorVideoMode.getResolutionY();

		if (depthWidth == colorWidth &&
			depthHeight == colorHeight)	{
			width = depthWidth;
			height = depthHeight;
		} else {
			printf("PrimeSense | Error - expect color and depth to be in same resolution: D: %dx%d, C: %dx%d\n",
				depthWidth, depthHeight,
				colorWidth, colorHeight);
			return openni::STATUS_ERROR;
		}
	} else if (depthStream.isValid())	{
		depthVideoMode = depthStream.getVideoMode();
		width = depthVideoMode.getResolutionX();
		height = depthVideoMode.getResolutionY();
	} else if (colorStream.isValid())	{
		colorVideoMode = colorStream.getVideoMode();
		width = colorVideoMode.getResolutionX();
		height = colorVideoMode.getResolutionY();
	} else {
		printf("PrimeSense | Error - expects at least one of the streams to be valid...\n");
		return openni::STATUS_ERROR;
	}

	openni::Status rc = openni::STATUS_OK;
	rc = initHandTracker();
	if (rc != openni::STATUS_OK)
		return openni::STATUS_ERROR;

	rc = initGrabDetector();
	if (rc != openni::STATUS_OK)
		return openni::STATUS_ERROR;

	if (userTracker.create(&device) != nite::STATUS_OK)	{
		return openni::STATUS_ERROR;
	}

	return openni::STATUS_OK;
}

// Initialize hand tracker
openni::Status PrimeSense::initHandTracker()
{
	//Initialize NiTE
	nite::Status niteStat = nite::STATUS_OK;
	niteStat = handTracker.create(&device);
	if (niteStat != nite::STATUS_OK) {
		printf("PrmeSense | Error initializing NiTE hand tracker: %d", (int)niteStat);
		return openni::STATUS_ERROR;
	}

	//Detect Click/Wave gestures
	//handTracker.startGestureDetection(nite::GESTURE_CLICK);
	handTracker.startGestureDetection(nite::GESTURE_WAVE);

	return openni::STATUS_OK;
}

//Initializes new GrabDetector object and events
openni::Status PrimeSense::initGrabDetector()
{
	//Create object
	grabDetector = PSLabs::CreateGrabDetector(device);
	if (grabDetector == NULL || grabDetector->GetLastEvent(NULL) != openni::STATUS_OK)
	{
		printf("PrimeSense | Error - cannot initialize grab detector: status %d \n", grabDetector->GetLastEvent(NULL));
		return openni::STATUS_ERROR;
	}

	//Initialize GrabDetector
	grabListener = new GrabEventListener(this);
	grabDetector->AddListener(grabListener);

	return openni::STATUS_OK;
}

void PrimeSense::updateUserTracker(float dt) {
	nite::UserTrackerFrameRef userTrackerFrame;
	nite::Status rc = userTracker.readFrame(&userTrackerFrame);
	if (rc != nite::STATUS_OK) {
		printf("PrimeSense | User tracker readFrame failed\n");
		return;
	}

	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
	for (int i = 0; i < users.getSize(); ++i) {
		const nite::UserData& user = users[i];

		if (user.isNew()) {
			printf("PrimeSense | New user detected: Assigning Id %d\n", user.getId());
		} else if (user.isLost()) {
			printf("PrimeSense | User %d lost.\n", user.getId());
			
			// Lost user currently being tracked.
			if (trackingUser && user.getId() == trackingUserId)
				stopTrackingUser();
		} else if (user.isVisible() && trackingUser && user.getId() == trackingUserId) {
			lastVisibleTime = dt;
		} else if (!user.isVisible() && (dt - lastVisibleTime) >= DELTA_TIME_FOR_NOT_VISIBLE_STOP && trackingUser && user.getId() == trackingUserId) {
			// Stop tracking if the user is not visible for 1 second.
			stopTrackingUser();
		}

		// Maybe use this to check the depth of the detected gesture as well
		//openni::VideoFrameRef depthFrame = userTrackerFrame.getDepthFrame();
		//const float* arr = (float*) depthFrame.getData();

		// Check if the person performed a wave gesture
		bool waved = checkGestureForUser(user);

		// Check if we are tracking a user or not
		if (!trackingUser && waved) {
			// No person being tracked and this person waved. Start tracking skeleton
			userTracker.startSkeletonTracking(user.getId());
			startedTrackingUser = true;
			trackingUser = true;
			trackingUserId = user.getId();
			startedTrackingTime = dt;
			printf("PrimeSense | Started tracking user %d!\n", trackingUserId);			
		}
		
		if (trackingUser && user.getId() == trackingUserId) {
			// We are tracking a user and its the user we are tracking

			// Check if the user waved. If he waved and 5 seconds passed when we started tracking the user, stop the tracking.
			if (waved && (dt - startedTrackingTime) >= DELTA_TIME_FOR_WAVE_STOP) {
				stopTrackingUser();
			}

			userReady = false;
			switch (user.getSkeleton().getState()) {
			case nite::SKELETON_TRACKED:
				//printf("PrimeSense | Tracking user %d!\n", user.getId());
				// THIS IS WERE WE UPDATE HAND POSITION ETC.
				userReady = true;
				updateUserData(user);
				break;
			case nite::SKELETON_CALIBRATING:
				//printf("PrimeSense | User %d: Calibrating skeleton...\n", user.getId());
				break;
			case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
			case nite::SKELETON_CALIBRATION_ERROR_HANDS:
			case nite::SKELETON_CALIBRATION_ERROR_LEGS:
			case nite::SKELETON_CALIBRATION_ERROR_HEAD:
			case nite::SKELETON_CALIBRATION_ERROR_TORSO:
				//printf("PrimeSense | Calibration of skeleton Failed... \n");
				break;
			case nite::SKELETON_NONE:
				//printf("PrimeSense | User %d: Not tracking skeleton.\n", user.getId());
				break;
			}			
		}
	}
}

void PrimeSense::updateUserData(const nite::UserData& user) {
	userInfo.headPos = user.getSkeleton().getJoint(nite::JOINT_HEAD).getPosition();
	userInfo.headOrientation = user.getSkeleton().getJoint(nite::JOINT_HEAD).getOrientation();
	userInfo.torsoPos = user.getSkeleton().getJoint(nite::JOINT_TORSO).getPosition();
	userInfo.torsoOrientation = user.getSkeleton().getJoint(nite::JOINT_TORSO).getOrientation();
	userInfo.neckPos = user.getSkeleton().getJoint(nite::JOINT_NECK).getPosition();
	userInfo.neckOrientation = user.getSkeleton().getJoint(nite::JOINT_NECK).getOrientation();
	userInfo.leftHandPos = user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition();
	userInfo.rightHandPos = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition();
}

bool PrimeSense::checkGestureForUser(const nite::UserData& user) {
	// Calculate bounding box for the user. This will be in "depth space" (z coordinate always zero).
	nite::BoundingBox box = user.getBoundingBox();
	//float boxX, boxY;
	//nite::Status rc = userTracker.convertDepthCoordinatesToJoint(box.min.x, box.min.y, 1.f, &boxX, &boxY);
	//printf("User %d: min(%f,%f,%f) max (%f,%f,%f)\n", user.getId(), box.min.x, box.min.y, box.min.z, box.max.x, box.max.y, box.max.z);
	

	// Read hand frame from hand tracker
	nite::HandTrackerFrameRef handFrame;
	if (handTracker.readFrame(&handFrame) != nite::STATUS_OK) {
		printf("PrimeSense | Hand tracker readFrame failed\n");
		return false;
	}
	
	// Check if there where gestures detected in the frame
	const nite::Array<nite::GestureData>& gestures = handFrame.getGestures();
	for (int i = 0; i < gestures.getSize(); ++i) {
		// Check if gesture is completed
		if (gestures[i].isComplete()) {
			// Get position of gesture in 3D coordinates
			const nite::Point3f& gesturePos = gestures[i].getCurrentPosition();

			// Convert them to "depth space"
			float gestureDepthX, gestureDepthY;
			nite::Status rc = userTracker.convertJointCoordinatesToDepth(gesturePos.x, gesturePos.y, gesturePos.z, &gestureDepthX, &gestureDepthY);

			switch (gestures[i].getType()) {
			case nite::GESTURE_CLICK:
				break;
			case nite::GESTURE_WAVE:
				if (gestureDepthX >= box.min.x - 50 && gestureDepthX <= box.max.x + 50 &&
					gestureDepthY >= box.min.y - 50 && gestureDepthY <= box.max.y + 50) {
						printf("PrimeSense | User %d performed wave gesture.\n", user.getId());
						return true;
				}
				break;
			case nite::GESTURE_HAND_RAISE:
				break;
			}
		}
	}

	return false;
}

void PrimeSense::stopTrackingUser() {
	userTracker.stopSkeletonTracking(trackingUserId);
	printf("PrimeSense | Stopped tracking user %d.\n", trackingUserId);

	trackingUser = false;
	trackingUserId = -1;
	stoppedTrackingUser = true;

	// Reset gesture in case a gesture was detected when hand was lost.
	gesture = NONE;
	userLost = true;
}

void PrimeSense::checkGesture(float dt) {
	nite::Point3f rightHandPos = userInfo.rightHandPos;
	nite::Point3f leftHandPos = userInfo.leftHandPos;
	nite::Point3f torsoPos = userInfo.torsoPos;

	float rightDist = std::abs(rightHandPos.x - torsoPos.x);
	float leftDist = std::abs(leftHandPos.x - torsoPos.x);

	if (leftDist < rightDist && rightDist >= ARMSTRETCH_THRESHOLD) {
		printf("PrimeSense | User %d performed a swipe right gesture.\n", trackingUserId);
		gesture = SWIPE_RIGHT;
	}
	else if (leftDist >= ARMSTRETCH_THRESHOLD) {
		printf("PrimeSense | User %d performed a swipe left gesture.\n", trackingUserId);
		gesture = SWIPE_LEFT;
	} else {
		gesture = NONE;
	}


}

void PrimeSense::processGrabEvent(PSLabs::IGrabEventListener::GrabEventType type)
{
	switch (type) {
	case PSLabs::IGrabEventListener::GRAB_EVENT:
		printf("PrimeSense | Grab detected.\n");
		grabbing = true;
		break;
	case PSLabs::IGrabEventListener::RELEASE_EVENT:
		printf("PrimeSense | Release detected.\n");	
		grabbing = false;
		break;
	case PSLabs::IGrabEventListener::NO_EVENT:
		printf("PrimeSense | 'No Event' detected.\n");
		break;
	}
}

float PrimeSense::distance(nite::Point3f p1, nite::Point3f p2) {
	return std::sqrtf(std::powf(p1.x - p2.x, 2) + std::powf(p1.y - p2.y, 2) + std::powf(p1.z - p2.z, 2));
}