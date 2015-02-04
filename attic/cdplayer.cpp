#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::auto_ptr;
#include <vector>
using std::vector;
#include <SDL/SDL.h>
#include "cdplayer.hpp"

int main(int argc, char* argv[]) {
auto_ptr<CDPlayer> pCDPlayer(new CDPlayer);

//putenv("SDL_AUDIODRIVER=dma");

	// welcome the user and find pertinent information
	cout << "Loading Malik CD Player..." << endl;
	short int intNumberOfDrives = pCDPlayer->getDriveCount();
	if (intNumberOfDrives > 0) {
		cout << "Scanning for cdroms..." << intNumberOfDrives << " found." << endl << endl;
	} else {
		cout << "Scanning for cdroms...Error:  No CD-Roms found on system." << endl;
		return EXIT_FAILURE;
	}
	
	// find out which drive they want to play the CD on
	string strBuffer;
	short int intBuffer;
	cout << "Which would you like to use for music?" << endl;
	for (short int x = 0; x != intNumberOfDrives; x++) {
		cout << (x + 1) << ")  " << pCDPlayer->getRomName(x) << endl;
	}
	
	do {
		cout << "enter number:  "; getline(cin, strBuffer, '\n');
	
		// the number entered
		intBuffer = (int)strBuffer[0] - 48;
	} while (intBuffer > intNumberOfDrives || intBuffer <= 0);
	
	// open the cd-rom so it can be access
	intBuffer--; // converts chosen number to how SDL understands it
	if (pCDPlayer->open(intBuffer) == false) {
		cout << "Error:  " << SDL_GetError() << endl;
		return EXIT_FAILURE;
	}
	
	char chrBuffer;
	bool blnRunning = true;
	while (blnRunning == true) {
		// update the CD's status
		if (pCDPlayer->updateStatus() == false) {
			cout << "Error:  Unable to read data from disk.  Please, make sure you have a CD in the cd-rom." << SDL_GetError() << endl;
			return EXIT_FAILURE;
		}
	
		// printing menu and get choice
		cout << endl << "Choose an action:" << endl;
		cout << "1) Play" << endl;
		cout << "2) Pause" << endl;
		cout << "3) Stop" << endl;
		cout << "4) Eject" << endl;
		cout << "5) Exit" << endl;
		
		while(chrBuffer != '1' && chrBuffer != '2' && chrBuffer != '3' && chrBuffer != '4' && chrBuffer != '5') {
			cout << "enter number:  "; getline(cin, strBuffer, '\n');
			chrBuffer = strBuffer[0];
		}

		// handle the action needed
		switch (chrBuffer) {
			case '1':
				cout << "There are " << pCDPlayer->getNumTracks() << " tracks..." << endl;
				cout << "Which would you like to play?" << endl;
				cout << "enter number:  "; getline(cin, strBuffer, '\n');
				cout << "Attempting to play track: " << (int)strBuffer[0] - 49 << endl;
				cout << "Playback status: " << pCDPlayer->play((int)strBuffer[0] - 49) << endl;
				break;
			case '2':
				if (pCDPlayer->IsPlaying() == true) {
					cout << "Pausing play..." << endl;
					pCDPlayer->pause();
				}
				
				cout << "Press enter key to unpause..." << endl;
				getchar();
				pCDPlayer->resume();
				break;
			case '3':
				if (pCDPlayer->stop() == true) {
					cout << "Stopped play..." << endl;
				}
				break;
			case '4':
				cout << "Ejecting cd..." << endl;
				pCDPlayer->eject();
				break;
			case '5':
				blnRunning = false;
				break;
		}
	}
	
	// close the cd-rom handle we opened previously
	pCDPlayer->close();
	
	return EXIT_SUCCESS;
}
