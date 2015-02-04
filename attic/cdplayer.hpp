#ifndef CDPLAYER_HPP
#define CDPLAYER_HPP

#include <iostream>
#include <SDL/SDL.h>
using std::string;

	class CDPlayer{
		public:
		CDPlayer()
			{
				// initializing SDL's CD-Rom subsystem
				SDL_Init(SDL_INIT_CDROM);
				
				// uninitializes it automatically
				atexit(SDL_Quit);
			}
		~CDPlayer() { }
		bool close()
			{
				SDL_CDClose(cdData);
			}
		bool eject()
			{
				if (cdData == NULL) return 0;
				if (SDL_CDEject(cdData) == -1) return false;
				return true;
			}
		bool isEmpty() const
			{
				if (cdData == NULL) return false;
				else if (cdData->status != CD_TRAYEMPTY) return false;
						
				return true;
			}
		bool isError() const
			{
				if (cdData == NULL) return false;
				else if (cdData->status != CD_ERROR) return false;
						
				return true;
			}
		bool isPlaying() const
			{
				if (cdData == NULL) return false;
				else if (cdData->status != CD_PLAYING) return false;		
				return true;
			}
		bool isPaused() const
			{
				if (cdData == NULL) return false;
				else if (cdData->status != CD_PAUSED) return false;
						
				return true;
			}
		bool isStopped() const
			{
				if (cdData == NULL) return false;
				else if (cdData->status != CD_STOPPED) return false;
						
				return true;
			}
		short int getDriveCount() const 
			{ 
				return SDL_CDNumDrives(); 
			}
		short int getNumTracks() const
			{
				if (cdData == NULL) return 0;
				return (cdData->numtracks + 1);
			}
		const char* getRomName(short int intNumDrive) const
			{
				return SDL_CDName(intNumDrive);
			}
		bool open(short int intNumDrive)
			{
				if ((cdData = SDL_CDOpen(intNumDrive)) == NULL) return false;
				return true;
			}
		bool pause()
			{
				if (cdData == NULL) return 0;
				if (SDL_CDPause(cdData) == -1) return false;
				return true;
			}
		bool play(short int intStartTrack)
			{
				if (cdData == NULL) return 0;
				if (SDL_CDPlayTracks(cdData, intStartTrack, 0, 1, 0) == -1) return false;
				//SDL_CDPlay(cdData,0,1000);
				return true;
			}
		bool resume()
			{
				if (cdData == NULL) return 0;
				if (SDL_CDResume(cdData) == -1) return false;
				return true;
			}
		bool stop()
			{
				if (cdData == NULL) return 0;
				if (SDL_CDStop(cdData) == -1) return false;
				return true;
			}
		bool updateStatus()
			{
				if (CD_INDRIVE(SDL_CDStatus(cdData)) == false) return false;
				return true;
			}
		
		private:
		SDL_CD* cdData;
	};
#endif
