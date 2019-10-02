
#define MANUAL_SPEED 0.4
#define MANUAL_STEP 2
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
	
	ServoUpdater* sUpdater;
	MotionProfile mProfile;
	void driveAndAddPoints();
	void driveAndEditPoint(int stepNum);
	void printMenu();
	char getCommand();
	void editProfile();
	
};
