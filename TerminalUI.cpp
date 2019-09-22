

using namespace std;

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "ServoUpdater.h"
#include "MotionProfile.h"
#include "FileUtils.h"
#include "TerminalUI.h"


TerminalUI::TerminalUI(ServoUpdater* sUp) {

	sUpdater = sUp;	
	
}


void TerminalUI::driveToPosition() {
	bool running = true;

	system("clear");

	cout << "Use wasd to move the laser into position and enter to set. Enter Q when finished." << endl;
	
	sUpdater->setLaserOn();
	
	// Set terminal to raw mode 
	system("stty raw"); 
	//sUpdater.goToPos(0.5, 0.5, 1.0); 		// center the laser
	// Loop while the laser is driven around
	
	
	
	while (running) {
		// Wait for single character 
		char input = getchar(); 
		double x;
		string mInput;
		
		switch (input) 
		{			
			case 'd':
			x = sUpdater->getStepFromPos(sUpdater->getdestPosA());
			if (x < MAX_STEP) x = x + MANUAL_STEP; 	
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getPosFromStep(x), sUpdater->getdestPosB(), MANUAL_SPEED, NO_PAUSE);				
			break;
			
			case 's':
			x = sUpdater->getStepFromPos(sUpdater->getdestPosB());
			if (x < MAX_STEP) x = x + MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getPosFromStep(x), MANUAL_SPEED, NO_PAUSE);	
			break;
			
			case 'a':
			x = sUpdater->getStepFromPos(sUpdater->getdestPosA());
			if (x > MIN_STEP) x = x - MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getPosFromStep(x), sUpdater->getdestPosB(), MANUAL_SPEED, NO_PAUSE);	
								
			break;
			
			case 'w':
			x = sUpdater->getStepFromPos(sUpdater->getdestPosB());
			if (x > MIN_STEP) x = x - MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getPosFromStep(x), MANUAL_SPEED, NO_PAUSE);	
				
			break;
			
			case '\r':
			
			double spd, pause;
			
			system("stty cooked"); 	
			cout << endl << endl;			
			cout << "Enter speed (0.0 - 1.0):";
			cin >> mInput;
			spd = strtod(mInput.c_str(), NULL);
			
			cout << "Enter pause time in seconds (0.0 - 60.0):";
			cin >> mInput;
			pause = strtod(mInput.c_str(), NULL);
			
			mProfile.addStep(sUpdater->getcurPosA(), sUpdater->getcurPosB(), spd, pause);
			cout << "step added. numSteps =" << mProfile.numSteps << endl;
			system("stty raw"); 

			break;
			
			case 'Q':
				running=false;
				sUpdater->setLaserOff();				
			break;
		}		
	}

	// Reset terminal to normal "cooked" mode 
	system("stty cooked"); 	
	
	// clean up the screen	
	system("clear");		
}


void TerminalUI::printMenu() {
	
//	system("clear");
		
	cout << "Main Menu" << endl << endl;
	
	cout << "m - move laser / add profile points" << endl;
	cout << "p - Print profile" << endl;
	cout << "c - Clear profile" << endl;
	cout << "t - Test profile" << endl;	
	cout << "r - Run CatToy" << endl;
	cout << "s - Save profile" << endl;
	cout << "l - Load profile" << endl;
	cout << "q - quit program" << endl;
	cout << "1 - Test1" << endl;
	cout << "2 - Test2" << endl;
	cout << "3 - Test3" << endl;
	cout << "4 - Test4" << endl;
	
}

char TerminalUI::getCommand() {
	
	string inText;
	
	printMenu();
	
	
	while (true) {
		
		cout << "Command:";
		cin >> inText;
		
		
		if (inText.length() == 1) {
			
		// if input is a proper command return with it. Otherwise call it out and get another.
		
			if ((inText.front() == 'm') ||
				(inText.front() == 'p') ||
				(inText.front() == 'c') ||
				(inText.front() == 'r') ||
				(inText.front() == 't') ||
			
				(inText.front() == 's') ||
				(inText.front() == '1') ||
				(inText.front() == '2') ||
				(inText.front() == '3') ||
				(inText.front() == '4') ||
				
				(inText.front() == 'q') ||
				(inText.front() == 'l')) {
				return(inText.front());
			}
		}		
			
		cout << "Invalid command. Valid options are m, p, c, r, t, s, q and l" << endl;
			
	}
				
}

void TerminalUI::runUI() {
	
	char command;	
	bool running = true;
	FileUtils fileU;
	struct timespec tp;

	
	while (running) {
		
		command = getCommand();
		
		switch (command) {
			
			case 'm':
				driveToPosition();			// just a way to move the laser
				break;
			
			case 'p':
			
				break;
			
			case 'c':
			
				break;
				
			case 'r':
			
				int numCycles, sleepTime;
				
				cout << "Number of cycles per run:";
				cin >> numCycles;
				
				cout << "Sleep time (sec) between runs:";
				cin >> sleepTime;
			
				tp.tv_sec = 0;
				tp.tv_nsec = 1000000;
				
				while(true) {
					sUpdater->setLaserOn();							// turn the laser on and repeat the cycle
					for (int y=0; y<numCycles; y++) {
						for (int x = 0; x < mProfile.numSteps; x++) {
							
							sUpdater->goToPos(mProfile.pan[x], mProfile.tilt[x], mProfile.speed[x], mProfile.pause[x]);						
							cout << "Moving to step:" << x << " pan:" << mProfile.pan[x] <<
							 " tilt:" << mProfile.tilt[x]  << " at speed:" << mProfile.speed[x]  <<
							  " for pause:" << mProfile.pause[x] << endl;
							  
							while(!sUpdater->getmoveComplete()) {						
								nanosleep(&tp, NULL);						
							}						
							cout << "Finished move." << endl;				
						}
					}
					sUpdater->setLaserOff();		// turn the laser off and go to sleep for a while
					sleep(sleepTime);					
					
				}
					
				sUpdater->setLaserOff();
			
				break;
				
			case 't':
			
				sUpdater->setLaserOn();
			
				tp.tv_sec = 0;
				tp.tv_nsec = 1000000;
					for (int x = 0; x < mProfile.numSteps; x++) {
						
						sUpdater->goToPos(mProfile.pan[x], mProfile.tilt[x], mProfile.speed[x], mProfile.pause[x]);						
						cout << "Moving to step:" << x << " pan:" << mProfile.pan[x] <<
						 " tilt:" << mProfile.tilt[x]  << " at speed:" << mProfile.speed[x]  <<
						  " for pause:" << mProfile.pause[x] << endl;
						  
						while(!sUpdater->getmoveComplete()) {						
							nanosleep(&tp, NULL);						
						}						
						cout << "Finished move." << endl;				
					}
					
				sUpdater->setLaserOff();
			
				break;

			case 's':
				fileU.saveProfile(mProfile);
				cout << "Motion Profile Saved." << endl;			
				break;

			case 'l':		
				fileU.loadProfile(&mProfile);
				cout << "Motion Profile Loaded." << endl;		
				
				break;

			case 'q':
			cout << "Got into Q and quitting." << endl;
				running = false;
				break;
				
			case '1':
				sUpdater->goToPos(0.0, 0.0, 1.0, 0.0);
					
				break;
				
			case '2':
				sUpdater->goToPos(1.0, 1.0, 1.0, 0.0);
				break;

			case '3':
				sUpdater->goToPos(0.5, 0.5, 0.05, 0.0);
				break;
			case '4':
				sUpdater->goToPos(0, 1, 1.0, 0.0);
				break;
			}
	}
}
	


void TerminalUI::autoRunUI() {

	FileUtils fileU;
	struct timespec tp;
	
				fileU.loadProfile(&mProfile);
				cout << "Motion Profile Loaded." << endl;		

	
	
				int numCycles, sleepTime;
				
				numCycles = AUTORUN_PROFILE_CYCLES;	
				sleepTime = AUTORUN_SLEEP_TIME;
			
				tp.tv_sec = 0;
				tp.tv_nsec = 1000000;
				
				while(true) {
					sUpdater->setLaserOn();							// turn the laser on and repeat the cycle
					for (int y=0; y<numCycles; y++) {
						for (int x = 0; x < mProfile.numSteps; x++) {
							
							sUpdater->goToPos(mProfile.pan[x], mProfile.tilt[x], mProfile.speed[x], mProfile.pause[x]);						
							cout << "Moving to step:" << x << " pan:" << mProfile.pan[x] <<
							 " tilt:" << mProfile.tilt[x]  << " at speed:" << mProfile.speed[x]  <<
							  " for pause:" << mProfile.pause[x] << endl;
							  
							while(!sUpdater->getmoveComplete()) {						
								nanosleep(&tp, NULL);						
							}						
							cout << "Finished move." << endl;				
						}
					}
					sUpdater->setLaserOff();		// turn the laser off and go to sleep for a while
					sleep(sleepTime);					
					
				}

}
