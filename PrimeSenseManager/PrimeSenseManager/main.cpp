#include <fstream>

#include "NiTE.h"
#include "GrabDetector.h"

void ProcessGrab(PSLabs::IGrabEventListener::GrabEventType Type)
{
	printf("Got ");
	if (Type == PSLabs::IGrabEventListener::GRAB_EVENT)
		printf("Grab");
	else if (Type == PSLabs::IGrabEventListener::RELEASE_EVENT)
		printf("Release");
	else if (Type == PSLabs::IGrabEventListener::NO_EVENT)
		printf("No Event?!");
	printf(" event\n");
}

class GrabEventListener : public PSLabs::IGrabEventListener
{
public:
	GrabEventListener()	{}
	virtual void DLL_CALL ProcessGrabEvent(const EventParams& params)
	{
		ProcessGrab(params.Type);
	}
};


openni::VideoStream& depth = openni::VideoStream();
openni::VideoStream& color = openni::VideoStream();
openni::VideoFrameRef m_depthFrame = openni::VideoFrameRef();
openni::VideoFrameRef m_colorFrame = openni::VideoFrameRef();
int m_width = 0;
int	m_height = 0;

openni::Device&	device = openni::Device();
nite::HandTracker m_handTracker = nite::HandTracker();
nite::HandId m_lastHandID = NULL;

PSLabs::IGrabDetector* m_grabDetector = NULL;
GrabEventListener* m_grabListener = NULL;

//Initializes new GrabDetector object and events
openni::Status InitGrabDetector(void)
{
	//Create object
	m_grabDetector = PSLabs::CreateGrabDetector(device);
	if (m_grabDetector == NULL || m_grabDetector->GetLastEvent(NULL) != openni::STATUS_OK)
	{
		printf("Error - cannot initialize grab detector: status %d \n", m_grabDetector->GetLastEvent(NULL));
		return openni::STATUS_ERROR;
	}

	//Initialize GrabDetector
	m_grabListener = new GrabEventListener();
	m_grabDetector->AddListener(m_grabListener);

	return openni::STATUS_OK;
}

openni::Status InitNiTE(void)
{
	//Initialize NiTE
	nite::Status niteStat = nite::STATUS_OK;
	niteStat = m_handTracker.create(&device);
	if (niteStat != nite::STATUS_OK)
	{
		printf("Error initializing NiTE hand tracker: %d", (int)niteStat);
		return openni::STATUS_ERROR;
	}

	//Detect Click/Wave gestures
	m_handTracker.startGestureDetection(nite::GESTURE_CLICK);
	m_handTracker.startGestureDetection(nite::GESTURE_WAVE);

	return openni::STATUS_OK;

}


openni::Status Init(int argc, char **argv)
{
	openni::VideoMode depthVideoMode;
	openni::VideoMode colorVideoMode;

	if (depth.isValid() && color.isValid())
	{
		depthVideoMode = depth.getVideoMode();
		colorVideoMode = color.getVideoMode();

		int depthWidth = depthVideoMode.getResolutionX();
		int depthHeight = depthVideoMode.getResolutionY();
		int colorWidth = colorVideoMode.getResolutionX();
		int colorHeight = colorVideoMode.getResolutionY();

		printf("%dx%d | %dx%d\n", depthWidth, depthHeight, colorWidth, colorHeight);
		if (depthWidth == colorWidth &&
			depthHeight == colorHeight)
		{
			m_width = depthWidth;
			m_height = depthHeight;
		}
		else
		{
			printf("Error - expect color and depth to be in same resolution: D: %dx%d, C: %dx%d\n",
				depthWidth, depthHeight,
				colorWidth, colorHeight);
			return openni::STATUS_ERROR;
		}
	}
	else if (depth.isValid())
	{
		depthVideoMode = depth.getVideoMode();
		m_width = depthVideoMode.getResolutionX();
		m_height = depthVideoMode.getResolutionY();
	}
	else if (color.isValid())
	{
		colorVideoMode = color.getVideoMode();
		m_width = colorVideoMode.getResolutionX();
		m_height = colorVideoMode.getResolutionY();
	}
	else
	{
		printf("Error - expects at least one of the streams to be valid...\n");
		return openni::STATUS_ERROR;
	}

	if (InitGrabDetector() != openni::STATUS_OK)
		return openni::STATUS_ERROR;

	if (InitNiTE() != openni::STATUS_OK)
		return openni::STATUS_ERROR;

}

//This function updates the NiTE tracker and gesture detection and fills their results in the given arguments
void UpdateNiTETrackers(bool* handLost, bool* gestureComplete, bool* handTracked, float* handX, float* handY, float* handZ)
{
	*handLost = false;
	*gestureComplete = false;
	*handTracked = false;

	//Read hand frame from NiTE
	nite::HandTrackerFrameRef handFrame;

	if (m_handTracker.readFrame(&handFrame) != nite::STATUS_OK)
	{
		printf("readFrame failed\n");
		return;
	}

	//Get gestures from NiTE
	const nite::Array<nite::GestureData>& gestures = handFrame.getGestures();
	for (int i = 0; i < gestures.getSize(); ++i)
	{
		if (gestures[i].isComplete())
		{
			const nite::Point3f& position = gestures[i].getCurrentPosition();
			printf("Gesture %d at (%f,%f,%f)\n", gestures[i].getType(), position.x, position.y, position.z);

			//Start hand tracker
			m_handTracker.startHandTracking(gestures[i].getCurrentPosition(), &m_lastHandID);

			//Update data
			*gestureComplete = true;
			*handX = position.x;
			*handY = position.y;
			*handZ = position.z;
		}
	}

	//Track hand and update position from NiTE
	const nite::Array<nite::HandData>& hands = handFrame.getHands();
	for (int i = 0; i < hands.getSize(); ++i)
	{
		const nite::HandData& user = hands[i];

		if (!user.isTracking())
		{
			printf("Lost hand %d\n", user.getId());
			nite::HandId id = user.getId();
			*handLost = true;
		}
		else
		{
			if (user.isNew())
			{
				printf("Found hand %d\n", user.getId());
			}

			//Update grab detector
			*handTracked = true;
			*handX = user.getPosition().x;
			*handY = user.getPosition().y;
			*handZ = user.getPosition().z;
		}
	}
}


//This function updates the algorithms after a new frame has been read
void UpdateAlgorithm(void)
{
	depth.readFrame(&m_depthFrame);
	if (color.isValid())
		color.readFrame(&m_colorFrame);

	bool handLost = false, gestureComplete = false, handTracked = false;
	float handX, handY, handZ;

	//Update NiTE trackers and get their result
	UpdateNiTETrackers(&handLost, &gestureComplete, &handTracked, &handX, &handY, &handZ);

	if (m_grabDetector != NULL)
	{
		//If the hand is lost, we need to reset the grab detector
		if (handLost)
			m_grabDetector->Reset();
		//If a gesture is just complete, or the hand is already being tracked, we have valid coordinates and can set them to the detector
		else if (gestureComplete || handTracked)
			m_grabDetector->SetHandPosition(handX, handY, handZ);

		//Update algorithm with the newly read frames. We prefer both frames, but can work only with one
		if (m_depthFrame.isValid() && m_colorFrame.isValid())
			m_grabDetector->UpdateFrame(m_depthFrame, m_colorFrame);
		else if (m_depthFrame.isValid())
			m_grabDetector->UpdateFrame(m_depthFrame);
	}
}

int main(int argc, char** argv)
{
	openni::Status rc = openni::STATUS_OK;

	//openni::Device device;
	//openni::VideoStream depth, color;
	const char* deviceURI = openni::ANY_DEVICE;
	if (argc > 1)
	{
		deviceURI = argv[1];
	}

	rc = openni::OpenNI::initialize();

	if (rc != openni::STATUS_OK)
	{
		printf("OpenNI Initialization failed:\n%s\n", openni::OpenNI::getExtendedError());
		return -1;
	}

	if (nite::NiTE::initialize() != nite::STATUS_OK)
	{
		printf("NiTE2 Initialization failed!");
		return -1;
	}

	rc = device.open(deviceURI);
	if (rc != openni::STATUS_OK)
	{
		printf("Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
		openni::OpenNI::shutdown();
		return -1;
	}
	rc = device.setDepthColorSyncEnabled(true);

	openni::VideoMode videoMode;
	int resX = 640, resY = 480, fps = 30;

	rc = depth.create(device, openni::SENSOR_DEPTH);
	if (rc == openni::STATUS_OK)
	{
		videoMode = depth.getVideoMode();
		videoMode.setFps(fps);
		videoMode.setResolution(resX, resY);
		rc = depth.setVideoMode(videoMode);
		if (rc != openni::STATUS_OK)
		{
			printf("Couldn't set depth mode:\n%s\n", openni::OpenNI::getExtendedError());
			return -1;
		}

		rc = depth.start();
		if (rc != openni::STATUS_OK)
		{
			printf("Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			depth.destroy();
			return -1;
		}
	}
	else
	{
		printf("Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
		return -1;
	}


	rc = color.create(device, openni::SENSOR_COLOR);
	if (rc == openni::STATUS_OK)
	{
		videoMode = color.getVideoMode();
		videoMode.setFps(fps);
		videoMode.setResolution(resX, resY);
		rc = color.setVideoMode(videoMode);
		if (rc != openni::STATUS_OK)
		{
			printf("Couldn't set color mode:\n%s\n", openni::OpenNI::getExtendedError());
			return -1;
		}

		rc = color.start();
		if (rc != openni::STATUS_OK)
		{
			printf("Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
			color.destroy();
			return -1;
		}
	}
	else
	{
		printf("Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
		return -1;
	}

	//Set stream parameters
	depth.setMirroringEnabled(true);
	color.setMirroringEnabled(true);

	//Set registration. This is very important as we cannot use image stream if they are not registered
	rc = device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
	if (rc != openni::STATUS_OK)
	{
		printf("Couldn't set image to depth registration. Disabling image stream.\n%s\n", openni::OpenNI::getExtendedError());
		color.stop();
		color.destroy();
	}

	if (!depth.isValid())
	{
		printf("No valid depth stream. Exiting\n");
		openni::OpenNI::shutdown();
		return -1;
	}

	std::ifstream file("Data/grab_gesture.dat");
	if (!file)
	{
		printf("Cannot find \"Data/grab_gesture.dat\"");
		openni::OpenNI::shutdown();
		return -1;
	}
	file.close();

	rc = Init(argc, argv);
	if (rc != openni::STATUS_OK)
	{
		openni::OpenNI::shutdown();
		return -1;
	}

	while (true) {
		UpdateAlgorithm();
	}

}