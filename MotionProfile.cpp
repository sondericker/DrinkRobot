


#include <iostream>
#include <stdlib.h>
#include "MotionProfile.h"

using namespace std;


MotionProfile::MotionProfile() {
	
	numSteps = 0;
	
}


void MotionProfile::addStep(double inPosA, double inPosB, double inPosC, double inPosD, double inPosE, double stepSpeed, double stepPause) {
	cout << "Into add step\n";
	
	posA[numSteps] = inPosA;
	posB[numSteps] = inPosB;
	posC[numSteps] = inPosC;
	posD[numSteps] = inPosD;
	posE[numSteps] = inPosE;
	
	speed[numSteps] = stepSpeed;
	pause[numSteps] = stepPause;
	numSteps++;
	
}


void MotionProfile::editStep(double inPosA, double inPosB, double inPosC, double inPosD, double inPosE, double stepSpeed, double stepPause, int stepNum) {
	cout << "Into add step\n";
	
	posA[stepNum] = inPosA;
	posB[stepNum] = inPosB;
	posC[stepNum] = inPosC;
	posD[stepNum] = inPosD;
	posE[stepNum] = inPosE;
	
	speed[stepNum] = stepSpeed;
	pause[stepNum] = stepPause;
	
}



