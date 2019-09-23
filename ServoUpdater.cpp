
#include <iostream>
#include <unistd.h>
#include "PWMDriver.h"
#include <wiringPi.h>
#include <sys/time.h>
#include <cmath>

#include "ServoUpdater.h"


using namespace std;


// ServoUpdater
//
// This class controls the motion for 2 servo axis and drives them
// using a thread from startPosition1,2 to endPosition1,2 at speed.
//
// All positions are doubles in the range of 0.0 - 1.0 representing the
// travel of the servo, which is abstracted and simplified in this class
//
// speed is also a double 0.0 - 1.0 with 1.0 representing the top speed
// of the servo. Both servos will always arrive simultaneously.
//
// Calling goToPos(endPos1, endPos2, speed) will initiate a movement
// which will continue until isMoving = false, indicating the move is complete.




ServoUpdater::ServoUpdater() {
	
    wiringPiSetupGpio ();                   // setup for button read
	pinMode (BUTTON_PIN, INPUT);
	
	// init pwm driver which will handle both pwm outputs being used
	pwm.initPWM(PWM_HAT_ADDRESS);			// default i2c hat address
	pwm.setPWMFreq(PWM_FREQ);		// run the PWM at 100Hz	
		
	curPosA = 0.5;
	curPosB = 0.5;
	curPosC = 0.5;	
	curPosD = 0.5;	
	curPosE = 0.5;	
	destPosA = 0.5;
	destPosB = 0.5;
	destPosC = 0.5;
	destPosD = 0.5;
	destPosE = 0.5;
	
	destSpeed = 0.5;
	
	delayMicroseconds(450000);				// delay 450ms	
	moveComplete = true;

}

void ServoUpdater::start() {
	
	// initialize mutex lock
	pthread_mutex_init(&lock, NULL);
	// thread is started here via threadHelper
	int err = pthread_create(&myThread, NULL, threadHelper, this);
	
	if (err != 0)
	{
		cout << "Error in thread creation: " << err << endl;
    }
    
}

void ServoUpdater::stop() {
	
	running = false;
	
}

void* ServoUpdater::threadHelper(void* arg) {
	
	ServoUpdater* thisObject = reinterpret_cast<ServoUpdater*>(arg);
	
    thisObject->updater();
    
    cout << "**** ServoUpdater thread stopped! ****" << endl;

    return(0);
	
}

void ServoUpdater::updater() {
	
	struct timeval start, end;

	running = true;
	
	while (running) {
		
		//	cout << ".";
		// grab the time we started the update loop
		gettimeofday(&start, NULL);			// time of the first run
		
		// call the servo motion routine
		updateServos();
				
		// grab the time again and decide how long we want to sleep
		// pwm writes are significant
		gettimeofday(&end, NULL);
		
		long mtime = ((end.tv_sec) * 1000000000 + end.tv_usec * 1000) - ((start.tv_sec) * 1000000000 + start.tv_usec * 1000);		// number of nanoseconds
		struct timespec tp;
		tp.tv_sec = 0;
		tp.tv_nsec = (1000000000/UPDATER_FREQUENCY_HZ) - mtime;	// 1 billion nano seconds / update frequency - how long it took to run the loop		

		if (tp.tv_nsec < 0) {
			tp.tv_nsec = 1000;	// run time is limited by execution speed. Just wait 1us
		}
		
	//	cout << "Was gone for:" << mtime << " nanoseconds so sleeping for:" << tp.tv_nsec << " nanoseconds." << endl;		
		nanosleep(&tp, NULL);		// updater runs every 1/UPDATE_FREQUENCY_HZ seconds

	}
	
	// exiting thread!
	cout << "Exiting thread running = " << running << " running addr = " << &running << endl;
	
}

void ServoUpdater::goToPos(double posA, double posB, double posC, double posD, double posE, double speed, double pause) {
	
	pthread_mutex_lock(&lock);

	destPosA = posA;
	destPosB = posB;
	destPosC = posC;
	destPosD = posD;
	destPosE = posE;
	destSpeed = speed;
	destPause = pause;

//	cout << " CALLED destPosA = " << destPosA << " destPosB = " << destPosB << endl << endl;

	moveComplete = false;

	pthread_mutex_unlock(&lock);

}


void ServoUpdater::updateServos() {

	pthread_mutex_lock(&lock);
	
	// first, decide which axis has farther to go. It will move at the 
	// commanded speed and the other will scale to arrive at the same time.
	double distA = destPosA - curPosA;
	double distB = destPosB - curPosB;
	double distC = destPosC - curPosC;
	double distD = destPosD - curPosD;
	double distE = destPosE - curPosE;
	double dist = 0;
	
		
	// clamp to finish move if we're close
	if (getStepFromPosA(fabs(distA)) - MIN_STEP_A < (destSpeed * (STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED)) + STEPS_SLOWEST_SPEED) {
		curPosA = destPosA;
//		cout << "Arrived A" << endl;
	}
	
	if (getStepFromPosB(fabs(distB)) - MIN_STEP_B < (destSpeed * (STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED)) + STEPS_SLOWEST_SPEED) {
		curPosB = destPosB;
//		cout << "Arrived B" << endl;
	}	
	
	if (getStepFromPosC(fabs(distC)) - MIN_STEP_C < (destSpeed * (STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED)) + STEPS_SLOWEST_SPEED) {
		curPosC = destPosC;
//		cout << "Arrived B" << endl;
	}	

	if (getStepFromPosD(fabs(distD)) - MIN_STEP_D < (destSpeed * (STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED)) + STEPS_SLOWEST_SPEED) {
		curPosD = destPosD;
//		cout << "Arrived B" << endl;
	}	

	if (getStepFromPosE(fabs(distE)) - MIN_STEP_E < (destSpeed * (STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED)) + STEPS_SLOWEST_SPEED) {
		curPosE = destPosE;
//		cout << "Arrived B" << endl;
	}	
	
		// calculate distance to move this cycle in steps
	double cyclDist = ((STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED) * destSpeed) + STEPS_SLOWEST_SPEED;
	double stepA = getStepFromPosA(curPosA);
	double stepB = getStepFromPosB(curPosB);
	double stepC = getStepFromPosC(curPosC);
	double stepD = getStepFromPosD(curPosD);
	double stepE = getStepFromPosE(curPosE);					

	// if pan has farther to go
	if ((fabs(distA) >= fabs(distB)) && 
		(fabs(distA) >= fabs(distC)) &&
		(fabs(distA) >= fabs(distD)) &&
		(fabs(distA) >= fabs(distE))) 
	{
		dist = distA;
	} else 	if ((fabs(distB) >= fabs(distA)) && 
		(fabs(distB) >= fabs(distC)) &&
		(fabs(distB) >= fabs(distD)) &&
		(fabs(distB) >= fabs(distE))) 
	{
		dist = distB;
	} else 	if ((fabs(distC) >= fabs(distA)) && 
		(fabs(distC) >= fabs(distB)) &&
		(fabs(distC) >= fabs(distD)) &&
		(fabs(distC) >= fabs(distE))) 
	{
		dist = distC;
	} else if ((fabs(distD) >= fabs(distA)) && 
		(fabs(distD) >= fabs(distB)) &&
		(fabs(distD) >= fabs(distC)) &&
		(fabs(distD) >= fabs(distE))) 
	{
		dist = distD;
	} else 	if ((fabs(distE) >= fabs(distA)) && 
		(fabs(distE) >= fabs(distB)) &&
		(fabs(distE) >= fabs(distC)) &&
		(fabs(distE) >= fabs(distD))) 
	{
		dist = distE;
	} else {
		cout << "Something went wrong, can't find max dist ServoUpdater.CPP" << endl;
	}
	
	if (dist >= 0) {
		// positive direction case - move pan the full amount
		// skip moving if we are there already
		if (curPosA != destPosA) {
			curPosA = getPosFromStepA(stepA + ((distA/dist)*cyclDist));
		}			
		if (curPosB != destPosB) {
			curPosB = getPosFromStepB(stepB + ((distB/dist)*cyclDist));	// should take care of the sign automatically
		}			
		if (curPosC != destPosC) {
			curPosC = getPosFromStepC(stepC + ((distC/dist)*cyclDist));	// should take care of the sign automatically
		}			
		if (curPosD != destPosD) {
			curPosD = getPosFromStepD(stepD + ((distD/dist)*cyclDist));	// should take care of the sign automatically
		}			
		if (curPosE != destPosE) {
			curPosE = getPosFromStepE(stepE + ((distE/dist)*cyclDist));	// should take care of the sign automatically
		}									
		
	} else {
		// Negative direction case
		if (curPosA != destPosA) {		
			curPosA = getPosFromStepA(stepA - ((distA/dist)*cyclDist));
		}			
		if (curPosB != destPosB) {
			curPosB = getPosFromStepB(stepB - ((distB/dist)*cyclDist));	// should take care of the sign automatically
		}
		if (curPosC != destPosC) {
			curPosC = getPosFromStepC(stepC - ((distC/dist)*cyclDist));	// should take care of the sign automatically
		}			
		if (curPosD != destPosD) {
			curPosD = getPosFromStepD(stepD - ((distD/dist)*cyclDist));	// should take care of the sign automatically
		}			
		if (curPosE != destPosE) {
			curPosE = getPosFromStepE(stepE - ((distE/dist)*cyclDist));	// should take care of the sign automatically
		}			
		
	}
		
	// and finally write the corPos in Steps values to the pwm driver
	pwm.setPWM(0,0x00, getStepFromPosA(curPosA));				
	pwm.setPWM(1,0x00, getStepFromPosB(curPosB));
	pwm.setPWM(2,0x00, getStepFromPosC(curPosC));
	pwm.setPWM(3,0x00, getStepFromPosD(curPosD));
	pwm.setPWM(4,0x00, getStepFromPosE(curPosE));
	
	if ((curPosA == destPosA) && (curPosB == destPosB) && (curPosC == destPosC) && (curPosD == destPosD) && (curPosE == destPosE) && !moveComplete) {
		
		// before move is complete we have to sleep for the pause time
		usleep (destPause * 1000000.0);
		
		moveComplete = true;
	}
	
	
	pthread_mutex_unlock(&lock);

}


double ServoUpdater::getStepFromPosA(double pos) {
	double val = ((MAX_STEP_A - MIN_STEP_A)*pos) + MIN_STEP_A;
	return(val);
}
double ServoUpdater::getPosFromStepA(double step) {
	double val = (step - MIN_STEP_A) / (MAX_STEP_A - MIN_STEP_A);
	return(val);
}


double ServoUpdater::getStepFromPosB(double pos) {
	double val = ((MAX_STEP_B - MIN_STEP_B)*pos) + MIN_STEP_B;
	return(val);
}
double ServoUpdater::getPosFromStepB(double step) {
	double val = (step - MIN_STEP_B) / (MAX_STEP_B - MIN_STEP_B);
	return(val);
}


double ServoUpdater::getStepFromPosC(double pos) {
	double val = ((MAX_STEP_C - MIN_STEP_C)*pos) + MIN_STEP_C;
	return(val);
}
double ServoUpdater::getPosFromStepC(double step) {
	double val = (step - MIN_STEP_C) / (MAX_STEP_C - MIN_STEP_C);
	return(val);
}


double ServoUpdater::getStepFromPosD(double pos) {
	double val = ((MAX_STEP_D - MIN_STEP_D)*pos) + MIN_STEP_D;
	return(val);
}
double ServoUpdater::getPosFromStepD(double step) {
	double val = (step - MIN_STEP_D) / (MAX_STEP_D - MIN_STEP_D);
	return(val);
}


double ServoUpdater::getStepFromPosE(double pos) {
	double val = ((MAX_STEP_E - MIN_STEP_E)*pos) + MIN_STEP_E;
	return(val);
}
double ServoUpdater::getPosFromStepE(double step) {
	double val = (step - MIN_STEP_E) / (MAX_STEP_E - MIN_STEP_E);
	return(val);
}


// synchronized getter methods for needed data

double ServoUpdater::getdestPosA() {
	pthread_mutex_lock(&lock);
	double x = destPosA;
	pthread_mutex_unlock(&lock);
	return(x);
}

double ServoUpdater::getdestPosB() {
	pthread_mutex_lock(&lock);
	double x = destPosB;
	pthread_mutex_unlock(&lock);
	return(x);
}

double ServoUpdater::getdestPosC() {
	pthread_mutex_lock(&lock);
	double x = destPosC;
	pthread_mutex_unlock(&lock);
	return(x);
	
}double ServoUpdater::getdestPosD() {
	pthread_mutex_lock(&lock);
	double x = destPosD;
	pthread_mutex_unlock(&lock);
	return(x);
	
}double ServoUpdater::getdestPosE() {
	pthread_mutex_lock(&lock);
	double x = destPosE;
	pthread_mutex_unlock(&lock);
	return(x);
}

double ServoUpdater::getcurPosA() {
	pthread_mutex_lock(&lock);
	double x = curPosA;
	pthread_mutex_unlock(&lock);
	return(x);
}

double ServoUpdater::getcurPosB() {
	pthread_mutex_lock(&lock);
	double x = curPosB;
	pthread_mutex_unlock(&lock);
	return(x);
}

double ServoUpdater::getcurPosC() {
	pthread_mutex_lock(&lock);
	double x = curPosC;
	pthread_mutex_unlock(&lock);
	return(x);
}
double ServoUpdater::getcurPosD() {
	pthread_mutex_lock(&lock);
	double x = curPosD;
	pthread_mutex_unlock(&lock);
	return(x);
}
double ServoUpdater::getcurPosE() {
	pthread_mutex_lock(&lock);
	double x = curPosE;
	pthread_mutex_unlock(&lock);
	return(x);
}



bool ServoUpdater::getmoveComplete() {
	pthread_mutex_lock(&lock);
	bool x = moveComplete;
	pthread_mutex_unlock(&lock);
	return(x);
}

double ServoUpdater::getdestSpeed() {
	pthread_mutex_lock(&lock);
	double x = destSpeed;
	pthread_mutex_unlock(&lock);
	return(x);
}

bool ServoUpdater::getRunning() {
	pthread_mutex_lock(&lock);
	bool x = running;
	pthread_mutex_unlock(&lock);
	return(x);	
}

bool ServoUpdater::getButtonState() {
	
	// read pin
	if (digitalRead(BUTTON_PIN) == HIGH) {
		return(false);
	} else {
		cout << "Button pushed!" << endl;
		return(true);
	}
	
}
	





