#ifndef OOSDL_DEBUG_CPP
#define OOSDL_DEBUG_CPP

#include <iostream>
#include "Debug.hpp"
#include "OOSDL_CONFIG.hpp"

OOSDL::Debug::Debug(std::ostream* output, bool enabled)
#if OOSDL_DEBUG_ENABLED
	: output(output), enabled(enabled), debug(*this)
	{
		if (this->getOutputStream() == NULL)
		{
			this->setOutputStream(&std::cerr);
		}
	}
#else
	{}
#endif

void OOSDL::Debug::enable()
{
	#if OOSDL_DEBUG_ENABLED
		this->enabled = true;
	#endif
}

void OOSDL::Debug::disable()
{
	#if OOSDL_DEBUG_ENABLED
		this->enabled = false;
	#endif
}

bool OOSDL::Debug::isEnabled() const
{
	#if OOSDL_DEBUG_ENABLED
		return this->enabled;
	#else
		return false;
	#endif
}

std::ostream* OOSDL::Debug::getOutputStream()
{
	#if OOSDL_DEBUG_ENABLED
		return this->output;
	#else
		return NULL;
	#endif
}

void OOSDL::Debug::setOutputStream(std::ostream* output)
{
	#if OOSDL_DEBUG_ENABLED
		this->output = output;
	#endif
}

template <typename Data>
OOSDL::Debug& OOSDL::Debug::operator<<(const Data& data)
{
	// Não pode haver comandos de preprocessador aqui porque o compilador deve
	// checar sempre se a interface de Data é válida(deve compilar este método)
	// para evitar que a classe se comporte de maneira diferente com valores
	// diferentes da flag OOSDL_DEBUG_ENABLED.
	if (this->isEnabled() && this->getOutputStream() != NULL)
	{
		(*this->getOutputStream()) << data;
	}
	
	return (*this);
}

#endif
