

#define default_speed 0.8

class MotionProfile {
	
	public:
	MotionProfile();
	void addStep(double panPos, double tiltPos, double stepSpeed, double pause);
	int numSteps;
	double pan[1000];
	double tilt[1000];
	double speed[1000];
	double pause[1000];
	
	protected:
	
	private:

	
};
