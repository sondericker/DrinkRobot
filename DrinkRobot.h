




class DrinkRobot {

	public:		
	DrinkRobot();
	void startUp();
	void printTime();
	struct timeval start, end;	
	protected:
	
	private:
	ServoUpdater sUpdater;
};
