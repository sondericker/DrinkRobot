

#define default_speed 0.8

class MotionProfile {
	
	public:
	MotionProfile();
	void addStep(double inPosA, double inPosB, double inPosC, double inPosD, double inPosE, double stepSpeed, double pause);
	void editStep(double inPosA, double inPosB, double inPosC, double inPosD, double inPosE, double stepSpeed, double pause, int stepNum);
	
	int numSteps;
	double posA[1000];
	double posB[1000];
	double posC[1000];
	double posD[1000];
	double posE[1000];

	double speed[1000];
	double pause[1000];
	
	protected:
	
	private:

	
};
