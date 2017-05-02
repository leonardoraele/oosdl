#ifndef OOSDL_CDDrive_CPP
#define OOSDL_CDDrive_CPP

#include "CDDrive.hpp"
#include "SDLCDROMClient.cpp"
#include <SDL/SDL.h>
#include <stdexcept>
#include <climits>
#include <map>
#include <utility>
#include "OOSDL_CONFIG.hpp"
#include "no_drive_found.cpp"
#include "no_cd_in.cpp"
#include "cd_error.cpp"

#define OOSDL_CDDRIVE_INVALID_TRACK_MESSAGE			"Invalid track number."

std::map<int, std::pair<SDL_CD*, int>> OOSDL::CDDrive::cd_map;

void OOSDL::CDDrive::verifyCD() const
throw (no_cd_in)
{
	if (!this->CDIn())
	{
		throw no_cd_in();
	}
}

OOSDL::CDDrive::CDDrive(SDL_CD* cdrom, int index, int currentTrack)
: cdrom(cdrom), index(index), currentTrack(currentTrack)
{}

OOSDL::CDDrive::~CDDrive()
{
	SDL_CDClose(this->get_SDL_CD());
}

int OOSDL::CDDrive::getCDDriveCount()
{
	return SDL_CDNumDrives();
}

OOSDL::CDDrive OOSDL::CDDrive::getCDDrive(int index)
throw (no_drive_found)
{
	std::pair<SDL_CD*, int> params(NULL, 0);
	
	if (cd_map.find(index) != cd_map.end())
	{
		params = cd_map[index];
	}
	else
	{
		SDL_CD* cdrom = SDL_CDOpen(index);
		if (cdrom == NULL)
		{
			throw no_drive_found();
		}
		else
		{
			params.first = cdrom;
			params.second = 0;
		}
		
		cd_map[index] = std::make_pair(params.first, params.second);
	}
	
	CDDrive result(params.first, index, params.second);
	
	return result;
}

std::string OOSDL::CDDrive::getName() const
{
	return SDL_CDName(this->index);
}

OOSDL::CDDrive::DriveState OOSDL::CDDrive::getState() const
{
	CDDrive* unconst_this = const_cast<CDDrive*>(this);
	SDL_CD* pCD = unconst_this->get_SDL_CD();
	
	DriveState result = DriveState::None;
	
	switch (SDL_CDStatus(pCD))
	{
		case CD_TRAYEMPTY:
			result = DriveState::NoCD;
			break;
		case CD_STOPPED:
		case CD_PLAYING:
		case CD_PAUSED:
			result = DriveState::CDIn;
			break;
		case CD_ERROR:
			result = DriveState::Fail;
			break;
	};
	
	return result;
}

bool OOSDL::CDDrive::CDIn() const
{
	return (this->getState() == DriveState::CDIn);
}


void OOSDL::CDDrive::eject()
throw (cd_error)
{
	SDL_CD* pCD = this->get_SDL_CD();
	
	bool failed = SDL_CDEject(pCD);
	
	if (failed)
	{
		throw cd_error();
	}
}

OOSDL::CDDrive::CDState OOSDL::CDDrive::getCDState() const
throw (no_cd_in)
{
	verifyCD();
	
	CDDrive* unconst_this = const_cast<CDDrive*>(this);
	SDL_CD* pCD = unconst_this->get_SDL_CD();
	
	CDState result = CDState::None;
	
	switch (SDL_CDStatus(pCD))
	{
		case CD_STOPPED:
			result = CDState::Stopped;
			break;
		case CD_PLAYING:
			result = CDState::Playing;
			break;
		case CD_PAUSED:
			result = CDState::Paused;
			break;
	};
	
	return result;
}

int OOSDL::CDDrive::getCDTrackCount() const
throw (no_cd_in)
{
	return this->get_SDL_CD()->numtracks;
}

void OOSDL::CDDrive::play(int track)
throw (no_cd_in, std::logic_error)
{
	verifyCD();
	SDL_CD* pCD = this->get_SDL_CD();
	
	if (track < 0 || track > this->getCDTrackCount())
	{
		throw std::logic_error(OOSDL_CDDRIVE_INVALID_TRACK_MESSAGE);
	}
	
	// Escolhe a faixa que vai ser executada.
	if (track == 0)
	{
		// Se foi usado o parâmetro 0(zero), usa a faixa após a última faixa executada. (e já seta a faixa seguinte.)
		track = currentTrack++;
	}
	else
	{
		// Ajusta o valor de track para a notação SDL(primeira track é a 0 e não 1). E configura a currentTrack para ser a track logo após a que será executada.
		currentTrack = track--;
	}
	
	SDL_CDPlayTracks(pCD, track, 0, 1, INT_MAX);
}

void OOSDL::CDDrive::pause()
throw (no_cd_in)
{
	verifyCD();
	SDL_CD* pCD = this->get_SDL_CD();
	SDL_CDPause(pCD);
}

void OOSDL::CDDrive::resume()
throw (no_cd_in)
{
	verifyCD();
	SDL_CD* pCD = this->get_SDL_CD();
	SDL_CDResume(pCD);
}

void OOSDL::CDDrive::stop()
throw (no_cd_in)
{
	verifyCD();
	SDL_CD* pCD = this->get_SDL_CD();
	SDL_CDStop(pCD);
}

SDL_CD* OOSDL::CDDrive::get_SDL_CD()
{
	return this->cdrom;
}

const SDL_CD* OOSDL::CDDrive::get_SDL_CD() const
{
	CDDrive* unconst_this = const_cast<CDDrive*>(this);
	
	return unconst_this->get_SDL_CD();
}

#endif
