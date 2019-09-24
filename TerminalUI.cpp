

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


void TerminalUI::driveAndAddPoints() {
	bool running = true;

	system("clear");

	cout << "Use a:d, w:s, r:f. t:g, y:h to move arm into position and enter to set. Enter Q when finished." << endl;
	
	
	// Set terminal to raw mode 
	system("stty raw"); 
	
	// Loop while the laser is driven around
	
	while (running) {
		// Wait for single character 
		char input = getchar(); 
		double x;
		string mInput;
		
		switch (input) 
		{			
			case 'd':
			x = sUpdater->getStepFromPosA(sUpdater->getdestPosA());
			if (x < MAX_STEP_A) x = x + MANUAL_STEP; 	
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getPosFromStepA(x), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);				
			break;

			case 'a':
			x = sUpdater->getStepFromPosA(sUpdater->getdestPosA());
			if (x > MIN_STEP_A) x = x - MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getPosFromStepA(x), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
								
			break;			
			
			case 's':
			x = sUpdater->getStepFromPosB(sUpdater->getdestPosB());
			if (x < MAX_STEP_B) x = x + MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getPosFromStepB(x), sUpdater->getdestPosC(), 
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;
			

			
			case 'w':
			x = sUpdater->getStepFromPosB(sUpdater->getdestPosB());
			if (x > MIN_STEP_B) x = x - MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getPosFromStepB(x), sUpdater->getdestPosC(),  
				sUpdater->getdestPosD(),  sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
				
			break;
			
			case 'f':
			x = sUpdater->getStepFromPosC(sUpdater->getdestPosC());
			if (x < MAX_STEP_C) x = x + MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getPosFromStepC(x),  
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;		

			case 'r':
			x = sUpdater->getStepFromPosC(sUpdater->getdestPosC());
			if (x > MIN_STEP_C) x = x - MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getPosFromStepC(x),  
				sUpdater->getdestPosD(),  sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
				
			break;
			
			case 'g':
			x = sUpdater->getStepFromPosD(sUpdater->getdestPosD());
			if (x < MAX_STEP_D) x = x + MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				sUpdater->getPosFromStepD(x), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;		

			case 't':
			x = sUpdater->getStepFromPosD(sUpdater->getdestPosD());
			if (x > MIN_STEP_D) x = x - MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(),
				sUpdater->getPosFromStepD(x), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
				
			break;

			case 'h':
			x = sUpdater->getStepFromPosE(sUpdater->getdestPosE());
			if (x < MAX_STEP_E) x = x + MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				 sUpdater->getdestPosD(), sUpdater->getPosFromStepE(x), MANUAL_SPEED, NO_PAUSE);	
			break;		

			case 'y':
			x = sUpdater->getStepFromPosE(sUpdater->getdestPosE());
			if (x > MIN_STEP_E) x = x - MANUAL_STEP;
//			cout << "Step = " << x << endl;

			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(),
				sUpdater->getdestPosD(), sUpdater->getPosFromStepE(x), MANUAL_SPEED, NO_PAUSE);	
				
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
			
			mProfile.addStep(sUpdater->getcurPosA(), sUpdater->getcurPosB(), sUpdater->getcurPosC(), 
				sUpdater->getcurPosD(), sUpdater->getcurPosE(), spd, pause);
			cout << "step added. numSteps =" << mProfile.numSteps << endl;
			system("stty raw"); 

			break;
			
			case 'Q':
				running=false;
			break;
			
			default:
			
				cout << "Unknown char"<< endl;
			break;
		}		
	}

	// Reset terminal to normal "cooked" mode 
	system("stty cooked"); 	
	
	// clean up the screen	
	system("clear");		
}

void TerminalUI::driveAndEditPoint(int stepNum) {
	bool running = true;

	system("clear");

	cout << "Use a:d, w:s, r:f. t:g, y:h to move arm into position and e to set. Enter to go to next step" << endl << endl;
	
//	sUpdater->setLaserOn();
	
	// Set terminal to raw mode 
//	system("stty raw"); 
	system("echo off");
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
			x = sUpdater->getStepFromPosA(sUpdater->getdestPosA());
			if (x < MAX_STEP_A) x = x + MANUAL_STEP; 	
			sUpdater->goToPos(sUpdater->getPosFromStepA(x), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);				
			break;

			case 'a':
			x = sUpdater->getStepFromPosA(sUpdater->getdestPosA());
			if (x > MIN_STEP_A) x = x - MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getPosFromStepA(x), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
								
			break;			
			
			case 's':
			x = sUpdater->getStepFromPosB(sUpdater->getdestPosB());
			if (x < MAX_STEP_B) x = x + MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getPosFromStepB(x), sUpdater->getdestPosC(), 
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;
			
			case 'w':
			x = sUpdater->getStepFromPosB(sUpdater->getdestPosB());
			if (x > MIN_STEP_B) x = x - MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getPosFromStepB(x), sUpdater->getdestPosC(),  
				sUpdater->getdestPosD(),  sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;
			
			case 'f':
			x = sUpdater->getStepFromPosC(sUpdater->getdestPosC());
			if (x < MAX_STEP_C) x = x + MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getPosFromStepC(x),  
				sUpdater->getdestPosD(), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;		

			case 'r':
			x = sUpdater->getStepFromPosC(sUpdater->getdestPosC());
			if (x > MIN_STEP_C) x = x - MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getPosFromStepC(x),  
				sUpdater->getdestPosD(),  sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;
			
			case 'g':
			x = sUpdater->getStepFromPosD(sUpdater->getdestPosD());
			if (x < MAX_STEP_D) x = x + MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				sUpdater->getPosFromStepD(x), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;		

			case 't':
			x = sUpdater->getStepFromPosD(sUpdater->getdestPosD());
			if (x > MIN_STEP_D) x = x - MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(),
				sUpdater->getPosFromStepD(x), sUpdater->getdestPosE(), MANUAL_SPEED, NO_PAUSE);	
			break;

			case 'h':
			x = sUpdater->getStepFromPosE(sUpdater->getdestPosE());
			if (x < MAX_STEP_E) x = x + MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(), 
				 sUpdater->getdestPosD(), sUpdater->getPosFromStepE(x), MANUAL_SPEED, NO_PAUSE);	
			break;		

			case 'y':
			x = sUpdater->getStepFromPosE(sUpdater->getdestPosE());
			if (x > MIN_STEP_E) x = x - MANUAL_STEP;
			sUpdater->goToPos(sUpdater->getdestPosA(), sUpdater->getdestPosB(), sUpdater->getdestPosC(),
				sUpdater->getdestPosD(), sUpdater->getPosFromStepE(x), MANUAL_SPEED, NO_PAUSE);	
			break;
			
			case 'e':
			
			double spd, pause;
			
			system("stty cooked"); 	
			system("echo on");
			cout << endl << endl;			
			cout << "Enter speed (0.0 - 1.0):";
			cin >> mInput;
			spd = strtod(mInput.c_str(), NULL);
			
			cout << "Enter pause time in seconds (0.0 - 60.0):";
			cin >> mInput;
			pause = strtod(mInput.c_str(), NULL);
			
			mProfile.editStep(sUpdater->getcurPosA(), sUpdater->getcurPosB(), sUpdater->getcurPosC(), 
				sUpdater->getcurPosD(), sUpdater->getcurPosE(), spd, pause, stepNum);
			cout << "step edited. numSteps =" << mProfile.numSteps << endl;
			system("stty raw");
			system("echo off"); 
			running = false;
			break;
			
			case '\r':
			cout << "skipping point" << endl;
			running = false;
			
			default:
			
				cout << "Unknown char"<< endl;
			break;
		}		
	}

	// Reset terminal to normal "cooked" mode 
	system("echo on"); 
	system("stty cooked"); 	
	
	// clean up the screen	
	system("clear");		
}

void TerminalUI::printMenu() {
	
//	system("clear");
		
	cout << "Main Menu" << endl << endl;
	
	cout << "m - Move arm / add profile points" << endl;
	cout << "e - Edit profile" << endl;
	cout << "p - Print profile" << endl;
	cout << "c - Clear profile" << endl;
	cout << "t - Test profile" << endl;	
	cout << "s - Save profile" << endl;
	cout << "l - Load profile" << endl;
	cout << "q - quit program" << endl;

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
				(inText.front() == 'e') ||			
				(inText.front() == 'p') ||
				(inText.front() == 'c') ||
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
			
		cout << "Invalid command. Valid options are m, e, p, c, t, s, q and l" << endl;
			
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
				driveAndAddPoints();			// just a way to move the laser
				break;

			case 'e':					// edit and adjust profile
				editProfile();
				break;
							
			case 'p':
			
			for (int x=0; x< mProfile.numSteps; x++) {
				
				cout << mProfile.posA[x] << ", " << mProfile.posB[x] << ", " << mProfile.posC[x] << ", " << mProfile.posD[x] << ", " << mProfile.posE[x] << ", "
				 << mProfile.speed[x] << ", " << mProfile.pause[x] << endl;
				
			}
			
				break;
			
			case 'c':
			
			mProfile.numSteps = 0;
			cout << "Profile cleared." << endl;
			
				break;
				
			
			case 't':
			
			
			tp.tv_sec = 0;
			tp.tv_nsec = 1000000;
	
		
			// wait for the button to be pressed. Poll every ms
			
			cout << "Waiting for button press.." << endl;	
			
			while (!sUpdater->getButtonState()) {
				nanosleep(&tp, NULL);
			}								
			
			for (int x = 0; x < mProfile.numSteps; x++) {						
				sUpdater->goToPos(mProfile.posA[x], mProfile.posB[x], mProfile.posC[x], mProfile.posD[x], mProfile.posE[x], mProfile.speed[x], mProfile.pause[x]);						
					cout << "Moving to step:" << x << " posA:" << mProfile.posA[x] <<
					" posB:" << mProfile.posB[x]  << " posC:" << mProfile.posC[x]  << " posD:" << mProfile.posD[x]  << " posE:" << mProfile.posE[x]  << " at speed:" << mProfile.speed[x]  <<
					" for pause:" << mProfile.pause[x] << endl;						  
					
				while(!sUpdater->getmoveComplete()) {						
					nanosleep(&tp, NULL);						
				}	
									
				cout << "Finished move." << endl;				
				
			}
					
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
				sUpdater->goToPos(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				break;
			case '2':
				sUpdater->goToPos(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0);
				break;
			case '3':
				sUpdater->goToPos(0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 0.0);
				break;
			case '4':
				sUpdater->goToPos(0.5, 1.0, 0.5, 1.0, 0.5, 1.0, 0.0);
				break;
				
			}
	}
}
	


void TerminalUI::autoRunUI() {

	FileUtils fileU;
	
	fileU.loadProfile(&mProfile);
	cout << "Motion Profile Loaded." << endl;		


	struct timespec tp;
	tp.tv_sec = 0;
	tp.tv_nsec = 1000000;
	
	while(true) {
		
		// wait for the button to be pressed. Poll every ms
		while (!sUpdater->getButtonState()) {
			nanosleep(&tp, NULL);
		}
			
		// Button was pushed, run profile
		
		for (int x = 0; x < mProfile.numSteps; x++) {
			
			sUpdater->goToPos(mProfile.posA[x], mProfile.posB[x], mProfile.posC[x], mProfile.posD[x], mProfile.posE[x], mProfile.speed[x], mProfile.pause[x]);						
			cout << "Moving to step:" << x << " pan:" << mProfile.posA[x] <<
			 " tilt:" << mProfile.posB[x]  << " at speed:" << mProfile.speed[x]  <<
			  " for pause:" << mProfile.pause[x] << endl;
			  
			while(!sUpdater->getmoveComplete()) {						
				nanosleep(&tp, NULL);						
			}						
			cout << "Finished move." << endl;				
		}
		
	}

}

void TerminalUI::editProfile() {
	
	struct timespec tp;
	tp.tv_sec = 0;
	tp.tv_nsec = 1000000;
	
	for (int x=0; x<mProfile.numSteps; x++) {
		
		sUpdater->goToPos(mProfile.posA[x], mProfile.posB[x], mProfile.posC[x], mProfile.posD[x], mProfile.posE[x], mProfile.speed[x], mProfile.pause[x]);						
		cout << "Moving to step:" << x << " pan:" << mProfile.posA[x] <<
			" tilt:" << mProfile.posB[x]  << " at speed:" << mProfile.speed[x]  <<
			" for pause:" << mProfile.pause[x] << endl;
							  
		while(!sUpdater->getmoveComplete()) {						
			nanosleep(&tp, NULL);						
		}
		
		cout << "Finished move." << endl;
		
		driveAndEditPoint(x);
		
	}
	
}




