




class CatToy {

	public:	
	struct timeval start, end;
	
	CatToy();
	void startUp();
	void printTime();
	protected:
	
	private:
	ServoUpdater sUpdater;
};
