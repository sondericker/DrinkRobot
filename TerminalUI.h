
#define MANUAL_SPEED 0.2
#define MANUAL_STEP 3
#define NO_PAUSE 0.0
#define AUTORUN_PROFILE_CYCLES 2
#define AUTORUN_SLEEP_TIME 1800

class TerminalUI {

	public:
	TerminalUI(ServoUpdater* sUp);
	void autoRunUI();
	void runUI();
	
	protected:

	private:
	double panPos;
	int panPosTicks;
	double tiltPos;
	int tiltPosTicks;
	
	ServoUpdater* sUpdater;
	MotionProfile mProfile;
	void driveToPosition();
	void printMenu();
	char getCommand();
	
};
