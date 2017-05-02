#ifndef OOSDL_Lock_HPP
#define OOSDL_Lock_HPP

#include <SDL/SDL.h>
#include <map>

/**
 * Lock é o mecanismo utilizado para sincronismo entre Threads.
 * Sempre que precisar sincronizar um bloco de execução para que este não seja
 * executado concorrentemente com outras threads crie um Lock. Os locks só
 * retornam de seu construtor ou do método lock() quando conseguem efetivamente
 * sincronizar a execução. Para separar diferentes blocos seguros são utilizados
 * ponteiros para identificação de blocos de código concorrentes. O fim do bloco
 * sincronizado é determinado pela destrução do objeto Lock ou pela invocação do
 * método unlock().
 *
 * Exemplo de uso:
 *
 * {
 *     instruções não sincronizadas
 *     ...
 *     Lock mutex(this);
 *     instruções sincronizadas pelo ponteiro this
 *     ...
 *     mutex.unlock();
 *     instruções não sincronizadas
 * }
 *
 */
namespace OOSDL {
	class Lock {
		const void* key;
		bool locked;
		
		static std::map<const void*, SDL_mutex*> mutex_map;
	public:
		Lock(const void*, bool locked = true);	// Cria uma trava de sincronização que usa o ponteiro como identificador. O parâmetro locked define se a trava inicia trancada.
		virtual ~Lock();						// Chama unlock() internamente quando é destruído.
	
		virtual bool isLocked() const;			// Verifica se esta trava encontra-se atualmente trancada ou não.
		virtual void lock();					// Recupera a trava de sincronização. Este método não retorna até que todas as outras threads que possuiam a trava anteriormente a liberem.
		virtual void unlock();					// Libera esta trava de sincronização, permitendo que outra seja criada com o mesmo identificador.
	};
};
#endif
