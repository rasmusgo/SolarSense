#ifndef PRIME_SENSE_H
#define PRIME_SENSE_H

#include <fstream>

#include "NiTE.h"
#include "GrabDetector.h"

#include <SFML/System.hpp>

#define DELTA_TIME_FOR_WAVE_STOP 5.f
#define DELTA_TIME_FOR_NOT_VISIBLE_STOP 1.f
#define ARMSTRETCH_THRESHOLD 400.f

class PrimeSense {
public:
	PrimeSense();
	~PrimeSense();
	
	class GrabEventListener : public PSLabs::IGrabEventListener	{
	public:
		GrabEventListener(PrimeSense* prime) : primeSense(prime) {}

		PrimeSense* primeSense;

		virtual void DLL_CALL ProcessGrabEvent(const EventParams& params) {
			primeSense->processGrabEvent(params.Type);
		}

	};

	enum Gesture {
		NONE,
		SWIPE_RIGHT,
		SWIPE_LEFT
	};

	void update();
	bool startedTracking();
	bool stoppedTracking();
	bool isGrabbing() { return grabbing; }
	void getHandPosition(float* handX, float* handY, float* handZ);
	Gesture getGesture() { return gesture; }

private:
	struct UserInfo {
		nite::Point3f headPos;
		nite::Quaternion headOrientation;
		nite::Point3f torsoPos;
		nite::Quaternion torsoOrientation;
		nite::Point3f neckPos;
		nite::Quaternion neckOrientation;
		nite::Point3f leftHandPos;
		nite::Point3f rightHandPos;
	};

	UserInfo userInfo;

	bool trackingUser, userReady, userLost, startedTrackingUser, stoppedTrackingUser, grabbing;
	int width, height;
	float lastVisibleTime, startedTrackingTime;
	Gesture gesture;

	sf::Clock clock;

	openni::Device device;
	openni::VideoStream depthStream;
	openni::VideoStream colorStream;
	openni::VideoFrameRef depthFrame;
	openni::VideoFrameRef colorFrame;

	nite::HandTracker handTracker;
	nite::UserTracker userTracker;
	nite::UserId trackingUserId;

	PSLabs::IGrabDetector* grabDetector;
	GrabEventListener* grabListener;

	openni::Status init();
	openni::Status initHandTracker();
	openni::Status initGrabDetector();

	void updateUserTracker(float dt);
	void updateUserData(const nite::UserData& user);
	bool checkGestureForUser(const nite::UserData& user);
	void processGrabEvent(PSLabs::IGrabEventListener::GrabEventType type);

	void stopTrackingUser();

	void checkGesture(float dt);

	float distance(nite::Point3f p1, nite::Point3f p2);
};

#endif // PRIME_SENSE_H