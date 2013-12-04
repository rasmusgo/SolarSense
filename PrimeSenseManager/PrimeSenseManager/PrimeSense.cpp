#include "PrimeSense.h"

PrimeSense::PrimeSense()
:	tracking(false), grabbing(false), width(0), height(0),
	//device(openni::Device()), depthStream(openni::VideoStream()), colorStream(openni::VideoStream()), 
	depthFrame(openni::VideoFrameRef()), colorFrame(openni::VideoFrameRef()),
	handTracker(nite::HandTracker()), lastHandID(NULL),
	grabDetector(NULL), grabListener(NULL) {

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
	depthStream.readFrame(&depthFrame);
	if (colorStream.isValid())
		colorStream.readFrame(&colorFrame);

	bool handTracked;

	//Update NiTE trackers and get their result
	updateHandTracker(&handTracked);

	// Update grab detector
	if (grabDetector != NULL) {
		//If the hand is lost, we need to reset the grab detector
		if (handLost)
			grabDetector->Reset();
		//If a gesture is just complete, or the hand is already being tracked, we have valid coordinates and can set them to the detector
		else if (newHand || handTracked)
			grabDetector->SetHandPosition(handX, handY, handZ);

		//Update algorithm with the newly read frames. We prefer both frames, but can work only with one
		if (depthFrame.isValid() && colorFrame.isValid())
			grabDetector->UpdateFrame(depthFrame, colorFrame);
		else if (depthFrame.isValid())
			grabDetector->UpdateFrame(depthFrame);
	}

	// Check gestures
	if (!grabbing) {
		checkGesture();
	} else {
		//distance = 0;
	}
}

void PrimeSense::getHandPosition(float* handX, float* handY, float* handZ) {
	(*handX) = this->handX;
	(*handY) = this->handY;
	(*handZ) = this->handZ;
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
	handTracker.startGestureDetection(nite::GESTURE_CLICK);
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
		printf("Error - cannot initialize grab detector: status %d \n", grabDetector->GetLastEvent(NULL));
		return openni::STATUS_ERROR;
	}

	//Initialize GrabDetector
	grabListener = new GrabEventListener(this);
	grabDetector->AddListener(grabListener);

	return openni::STATUS_OK;
}

//This function updates the NiTE hand tracker and gesture detection and fills their results in the given arguments
void PrimeSense::updateHandTracker(bool* handTracked) {
	newHand = false;
	handLost = false;
	*handTracked = false;

	//Read hand frame from NiTE
	nite::HandTrackerFrameRef handFrame;

	if (handTracker.readFrame(&handFrame) != nite::STATUS_OK) {
		printf("readFrame failed\n");
		return;
	}

	//Get gestures from NiTE
	const nite::Array<nite::GestureData>& gestures = handFrame.getGestures();
	for (int i = 0; i < gestures.getSize(); ++i) {
		if (gestures[i].isComplete()) {
			const nite::Point3f& position = gestures[i].getCurrentPosition();
			printf("PrimeSense | Gesture %d detected at (%f,%f,%f)\n", gestures[i].getType(), position.x, position.y, position.z);

			switch (gestures[i].getType()) {
			case nite::GESTURE_CLICK:
				break;
			case nite::GESTURE_WAVE:
				if (!tracking) {
					//Start hand tracker
					handTracker.startHandTracking(gestures[i].getCurrentPosition(), &lastHandID);
					tracking = true;
					newHand = true;
				} else {
					// if(deltaTime > 1)
					// stopTracking(gestures[i].getCurrentPosition());
				}
				break;
			case nite::GESTURE_HAND_RAISE:
				break;
			}

			//Update data
			//*gestureComplete = true;
			handX = position.x;
			handY = position.y;
			handZ = position.z;
		}
	}

	//Track hand and update position from NiTE
	const nite::Array<nite::HandData>& hands = handFrame.getHands();
	// hands.getSize() should always be <= 1
	for (int i = 0; i < hands.getSize(); ++i) {
		const nite::HandData& user = hands[i];

		if (user.isLost()) {
			printf("PrimeSense | Lost hand %d\n", user.getId());
			handLost = true;
			tracking = false;
		} else {
			if (user.isNew()) {
				printf("PrimeSense | Found hand %d\n", user.getId());
			}

			//Update data
			*handTracked = true;
			handX = user.getPosition().x;
			handY = user.getPosition().y;
			handZ = user.getPosition().z;
		}
	}
}

void PrimeSense::checkGesture() {
	//float distance = lastHandX - handX;
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