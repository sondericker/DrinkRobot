
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
	
    wiringPiSetupGpio ();                   // setup for laser control
	pinMode (21, OUTPUT);
	setLaserOff();	
	
	// init pwm driver which will handle both pwm outputs being used
	pwm.initPWM(PWM_HAT_ADDRESS);			// default i2c hat address
	pwm.setPWMFreq(PWM_FREQ);		// run the PWM at 100Hz	
		
	curPosA = 0.5;
	curPosB = 0.5;
	destPosA = 0.5;
	destPosB = 0.5;
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

void ServoUpdater::goToPos(double posA, double posB, double speed, double pause) {
	
	pthread_mutex_lock(&lock);

	destPosA = posA;
	destPosB = posB;
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
	
	
	// clamp to finish move if we're close
	if (getStepFromPos(fabs(distA)) - MIN_STEP < (destSpeed * (STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED)) + STEPS_SLOWEST_SPEED) {
		curPosA = destPosA;
//		cout << "Arrived A" << endl;
	}
	
	if (getStepFromPos(fabs(distB)) - MIN_STEP < (destSpeed * (STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED)) + STEPS_SLOWEST_SPEED) {
		curPosB = destPosB;
//		cout << "Arrived B" << endl;
	}	
	

	// if pan has farther to go
	if (fabs(distA) >= fabs(distB)) {
		// calculate distance to move this cycle in steps
		double cyclDist = ((STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED) * destSpeed) + STEPS_SLOWEST_SPEED;
		double stepA = getStepFromPos(curPosA);
		double stepB = getStepFromPos(curPosB);
		
		if (distA >= 0) {
			// positive direction case - move pan the full amount
			// skip moving if we are there already
			if (curPosA != destPosA) {
				curPosA = getPosFromStep(stepA + cyclDist);
			}
			
			// move tilt less by a scaling factor
			if (curPosB != destPosB) {
				curPosB = getPosFromStep(stepB + ((distB/distA)*cyclDist));	// should take care of the sign automatically
			}
		} else {
			// Negative direction case
			if (curPosA != destPosA) {		
				curPosA = getPosFromStep(stepA - cyclDist);
			}
			
			if (curPosB != destPosB) {
				curPosB = getPosFromStep(stepB - ((distB/distA)*cyclDist));	// should take care of the sign automatically
			}
		}
	// tilt has farther to go
	} else {
		double cyclDist = ((STEPS_FASTEST_SPEED - STEPS_SLOWEST_SPEED) * destSpeed) + STEPS_SLOWEST_SPEED;
		double stepA = getStepFromPos(curPosA);
		double stepB = getStepFromPos(curPosB);

		if (distB >= 0) {
			// positive direction case
			if (curPosB != destPosB) {
				curPosB = getPosFromStep(stepB + cyclDist);
			}
			
			if (curPosA != destPosA) {					
				curPosA = getPosFromStep(stepA + ((distA/distB)*cyclDist));	// should take care of the sign automatically
			}
		} else {
			// Negative direction case
			if (curPosB != destPosB) {		
				curPosB = getPosFromStep(stepB - cyclDist);	
			}
			
			if (curPosA != destPosA) {								
				curPosA = getPosFromStep(stepA - ((distA/distB)*cyclDist));	// should take care of the sign automatically
			}
		}
	}		
	
	// and finally write the corPos in Steps values to the pwm driver
	pwm.setPWM(0,0x00, getStepFromPos(curPosA));				// send them back to the beginning to start
	pwm.setPWM(1,0x00, getStepFromPos(curPosB));
	
	if ((curPosA == destPosA) && (curPosB == destPosB) && !moveComplete) {
		
		// before move is complete we have to sleep for the pause time
		
		
		usleep (destPause * 1000000.0);
		
		moveComplete = true;
	}
	
	
	pthread_mutex_unlock(&lock);

}


double ServoUpdater::getStepFromPos(double pos) {
	double val = ((MAX_STEP - MIN_STEP)*pos) + MIN_STEP;
	return(val);
}


double ServoUpdater::getPosFromStep(double step) {
	double val = (step - MIN_STEP) / (MAX_STEP - MIN_STEP);
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

void ServoUpdater::setLaserOn() {
	pthread_mutex_lock(&lock);
    digitalWrite (21, HIGH);
	pthread_mutex_unlock(&lock);
}

void ServoUpdater::setLaserOff() {
	pthread_mutex_lock(&lock);	
    digitalWrite(21, LOW);
	pthread_mutex_unlock(&lock);    
}

	





