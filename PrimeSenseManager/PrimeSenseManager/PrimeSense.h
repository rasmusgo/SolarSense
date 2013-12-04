#ifndef PRIME_SENSE_H
#define PRIME_SENSE_H

#include <fstream>

#include "NiTE.h"
#include "GrabDetector.h"

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

	void update();
	bool startedTracking() { return newHand; }
	bool stoppedTracking() { return handLost; }
	bool isGrabbing() { return grabbing; }
	void getHandPosition(float* handX, float* handY, float* handZ);

private:
	bool tracking, newHand, handLost, grabbing;
	int width ;
	int	height;
	float handX, handY, handZ;

	openni::Device device;
	openni::VideoStream depthStream;
	openni::VideoStream colorStream;
	openni::VideoFrameRef depthFrame;
	openni::VideoFrameRef colorFrame;

	nite::HandTracker handTracker;
	nite::HandId lastHandID;

	PSLabs::IGrabDetector* grabDetector;
	GrabEventListener* grabListener;

	openni::Status init();
	openni::Status initHandTracker();
	openni::Status initGrabDetector();

	void updateHandTracker(bool* handTracked);
	void checkGesture();
	void processGrabEvent(PSLabs::IGrabEventListener::GrabEventType type);
};

#endif // PRIME_SENSE_H