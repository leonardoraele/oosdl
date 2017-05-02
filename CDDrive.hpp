#ifndef OOSDL_CDDrive_HPP
#define OOSDL_CDDrive_HPP

#include <SDL/SDL.h>
#include <string>
#include <stdexcept>
#include <map>
#include <utility>
#include "SDLCDROMClient.hpp"
#include "no_cd_in.hpp"
#include "no_drive_found.hpp"
#include "cd_error.hpp"

namespace OOSDL {
	class CDDrive : public SDLCDROMClient {
	public:
		enum class DriveState {None = 0
			, NoCD
			, CDIn
			, Fail
		,DriveStateEnd};
		enum class CDState {None = 0
			, Playing
			, Paused
			, Stopped
		,CDStateEnd};
	private:
		SDL_CD* cdrom;
		int currentTrack;
		int index;
		
		static std::map<int, std::pair<SDL_CD*, int>> cd_map;
		
		void verifyCD() const throw (no_cd_in);									// Verifica e levanta exceção caso não haja CD.
	public:
		CDDrive(SDL_CD* cdrom, int index, int currentTrack = 0);
		
		virtual ~CDDrive();
		static int getCDDriveCount();											// Verifica o número de drives de CD que a máquina possui.
		static CDDrive getCDDrive(int index) throw (no_drive_found);			// Recure uma isntância de CDDrive referente ao drive indicado.(a partir do 0)
		
		// Métodos de verificação.
		virtual std::string getName() const;									// Verifica o nome do drive. Ex: "/dev/cdrom" ou "E:"
		virtual DriveState getState() const;									// Verifica o estado atual deste drive de CD.
		virtual bool CDIn() const;												// Verifica se o drive tem um CD dentro. Equivalente à "getState() == DriveState::CDIn", existe apenas para facilitar esta verificação para os métodos de controle de execução do CD.
		
		// Ejetar o drive.
		virtual void eject() throw (cd_error);									// Ejeta o CD atualmente no drive ou abre o drive para o usuário inserir um CD.
		
		// Métodos de controle de execução do CD.
		// Todos estes métodos levantam exceção no_cd_in caso o CDDrive esteja
		// sem CD. Isto pode ser verificado com o método CDIn().
		virtual CDState getCDState() const throw (no_cd_in);					// Verifica o estado da execução do CD.
		virtual int getCDTrackCount() const throw (no_cd_in);					// Verifica o número de faixas que o CD possui.
		virtual void play(int track = 0) throw (no_cd_in, std::logic_error);	// Inicia execução da faixa especificada do CD(considerando a primeira faixa 1 e a última determinada pelo método getCDTrackCount()). Se track for 0, a faixa escolhida será a faixa logo após a última faixa executada. Se a faixa escolhida estiver fora do range de tracks do CD, uma instância de std::logic_error é levantada.
		virtual void pause() throw (no_cd_in);									// Pausa a execução de audio do CD. Se nenhuma faixa estiver em execução, o método não faz nada.
		virtual void resume() throw (no_cd_in);									// Continua uma faixa que foi pausada pelo método pause(). Se nenhuma faixa estiver pausada, o método não faz nada.
		virtual void stop() throw (no_cd_in);									// Pára uma faixa em execução. se nenhuma faixa estava em execução ou pausada, este método não faz nada.
		
		// Métodos SDL
		virtual SDL_CD* get_SDL_CD();
		virtual const SDL_CD* get_SDL_CD() const;
	};
};

#endif
