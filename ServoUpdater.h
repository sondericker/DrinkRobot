

#define UPDATER_FREQUENCY_HZ	50			// frequency the updater runs at
#define PWM_HAT_ADDRESS 		0x40		// address of PWM hat hardware
#define PWM_FREQ				50			// 20ms

#define NEUTRAL_POSA 0.5050
#define NEUTRAL_POSB 0.9300
#define NEUTRAL_POSC 0.860
#define NEUTRAL_POSD 0.600
#define NEUTRAL_POSE 0.5000

#define LOW_CENTER_VAL 	0.4950		// low value for which the continuous servo is turned off
#define HIGH_CENTER_VAL	0.5050		// high value for which the continuous servo is turned off

#include <pthread.h>
#include "PWMDriver.h"

#define STEPS_SLOWEST_SPEED 0.1
#define STEPS_FASTEST_SPEED 10.0
//#define MIN_STEP 210.0
//#define MAX_STEP 1010.0

#define MIN_STEP_A 105.0
#define MAX_STEP_A 505.0
#define MIN_STEP_B 105.0
#define MAX_STEP_B 505.0
#define MIN_STEP_C 105.0
#define MAX_STEP_C 505.0
#define MIN_STEP_D 105.0
#define MAX_STEP_D 505.0
#define MIN_STEP_E 105.0
#define MAX_STEP_E 505.0

#define BUTTON_PIN 20


class ServoUpdater {
	
	public:
	ServoUpdater();
	void start();
	void stop();
	void goToPos(double posA, double posB, double posC, double posD, double posE, double speed, double pause);	
	double getStepFromPosA(double pos);
	double getPosFromStepA(double step);
	double getStepFromPosB(double pos);
	double getPosFromStepB(double step);	
	double getStepFromPosC(double pos);
	double getPosFromStepC(double step);	
	double getStepFromPosD(double pos);
	double getPosFromStepD(double step);	
	double getStepFromPosE(double pos);
	double getPosFromStepE(double step);
	bool getButtonState();


	double getdestPosA();		// 0.0 - 1.0 pan destination position
	double getdestPosB();		// 0.0 - 1.0 tilt destination position
	double getdestPosC();		// 0.0 - 1.0 tilt destination position
	double getdestPosD();		// 0.0 - 1.0 tilt destination position
	double getdestPosE();		// 0.0 - 1.0 tilt destination position
	
	double getdestSpeed();		// 0.0 - 1.0 representing slowest to fastest speed
	double getcurPosA();			// pan current position
	double getcurPosB();			// tilt current position	
	double getcurPosC();			// tilt current position		
	double getcurPosD();			// tilt current position	
	double getcurPosE();			// tilt current position			
	bool getmoveComplete();	
	bool getRunning();
	bool getPWMRunState();
	void setPWMRunState(bool x);

	protected:
	
	private:
	void updater();							// looping thread method
	static void* threadHelper(void* arg);	// static method to launch thread
	void updateServos();					// method to actually update servo pwms
	pthread_t myThread;
	PWMDriver pwm;	
	bool pwmRunState;
	bool running;
	pthread_mutex_t lock;
	
	double destPosA;		// 0.0 - 1.0 pan destination position
	double destPosB;		// 0.0 - 1.0 tilt destination position
	double destPosC;		// 0.0 - 1.0 tilt destination position
	double destPosD;		// 0.0 - 1.0 tilt destination position
	double destPosE;		// 0.0 - 1.0 tilt destination position
	double destSpeed;		// 0.0 - 1.0 representing slowest to fastest speed
	double destPause;
	double curPosA;			// pan current position
	double curPosB;			// tilt current position	
	double curPosC;			// tilt current position	
	double curPosD;			// tilt current position	
	double curPosE;			// tilt current position	

	bool moveComplete;	

};
