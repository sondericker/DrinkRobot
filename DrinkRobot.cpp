

//  DrinkRobot - a drink stirring robot


#include <iostream>
#include <stdlib.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <unistd.h>
#include "MotionProfile.h"
#include "ServoUpdater.h"
#include "TerminalUI.h"
#include "DrinkRobot.h"


using namespace std;


int main(int argc, char **argv)
{
	DrinkRobot *drx = new DrinkRobot;
	drx->startUp();
	cout << "back from DrinkRobot, exiting!" << endl;;

	return 0;
}

DrinkRobot::DrinkRobot() {		

}

void DrinkRobot::startUp() {
	
	sUpdater.start();
	
	while (sUpdater.getRunning() != true) {
		cout<<".";
	}
	cout << endl;

	TerminalUI tUI(&sUpdater);		// this is a loop that runs until we quit
	tUI.runUI();
	
	sUpdater.stop();				// kill the updater thread
	cout << "exiting.." << endl;
	sUpdater.setLaserOff();
	exit(0);						// exit the application
	
}


void DrinkRobot::printTime () {
	
    struct timeval start, end;

    long mtime, seconds, useconds;    

    gettimeofday(&start, NULL);
    gettimeofday(&end, NULL);

//    seconds  = end.tv_sec  - start.tv_sec;
//    useconds = end.tv_usec - start.tv_usec; 
    
    seconds  = start.tv_sec;
    useconds = start.tv_usec;    

    mtime = ((seconds) * 1000000 + useconds);
    
    cout << "Time: " << mtime << endl;

   // printf("Elapsed time: %ld\n", mtime);
	
	
}


