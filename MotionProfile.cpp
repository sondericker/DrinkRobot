


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





