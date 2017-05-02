#ifndef OOSDL_Bitmap_CPP
#define OOSDL_Bitmap_CPP

#include "Bitmap.hpp"
#include "Font.cpp"
#include "TTFClient.cpp"
#include "sdl_error.cpp"
#include "OOSDL_CONFIG.hpp"

#define TTF_RenderMethod	TTF_RenderText_Solid
#define INVALID_COORDINATE	"A coordenada informada é inválida."

#define DEEPTH				32
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK				0x000000ff
#define GMASK				0x0000ff00
#define BMASK				0x00ff0000
#define AMASK				0xff000000
#else
#define RMASK				0xff000000
#define GMASK				0x00ff0000
#define BMASK				0x0000ff00
#define AMASK				0x000000ff
#endif

//std::map< std::string, std::map< int, TTF_Font* > > OOSDL::Bitmap::fontCache;

OOSDL::Bitmap::Bitmap(int width, int height, const Color& bitmap_color)
throw (sdl_error)
: surface(
		createSurface(0, width, height)
	)
	, background_color(NULL)
	, lock_flag(false)
	, TTFClient(NULL)
{
	// Tratamento de erros.
	if (this->surface == NULL) {
		throw sdl_error();
	}
	
	// Preenche o bitmap com a cor determinada.
	this->drawRect(this->asRect(), bitmap_color);
}

OOSDL::Bitmap::Bitmap(const std::string& filename, const Color& background_color)
throw (sdl_error)
: surface(
		SDL_LoadBMP(filename.c_str())
	)
	, background_color(new Color(background_color))
	, lock_flag(false)
	, TTFClient(NULL)
{
	// Tratamento de erros.
	if (this->surface == NULL) {
		throw sdl_error();
	}
}

OOSDL::Bitmap::Bitmap(SDL_Surface* surface)
: surface(surface), background_color(NULL), lock_flag(true), TTFClient(NULL)
{
	this->unlock();
}

OOSDL::Bitmap::Bitmap(const Bitmap& other)
throw (sdl_error)
: surface(createSurface(0, other.getWidth(), other.getHeight()))
	, background_color(NULL)
	, lock_flag(false)
	, TTFClient(NULL)
{
	// Verifica por erros na criação do surface.
	if (this->surface == NULL)
	{
		throw sdl_error();
	}
	
	// Copia a cor de plano de fundo.
	if (other.background_color != NULL)
	{
		this->background_color = new Color(*other.background_color);
	}
	
	// Copia o conteúdo de um bitmap para o outro.
	this->drawBitmap(other);
}

OOSDL::Bitmap::~Bitmap()
{
	this->dispose();
}

void OOSDL::Bitmap::dispose()
{
	if (background_color != NULL)
	{
		delete background_color;
		background_color = NULL;
	}
	if (this->isValid())
	{
		SDL_FreeSurface(this->get_SDL_Surface());
		this->surface = NULL;
	}
}

void OOSDL::Bitmap::lock() {
	if (!this->isLocked() && SDL_MUSTLOCK(this->get_SDL_Surface()))
	{
		int error = SDL_LockSurface(this->get_SDL_Surface());
		if (error < 0)
		{
			SDL_GetError(); // TODO Tratar.
		}
		else
		{
			this->lock_flag = true;
		}
	}
}

void OOSDL::Bitmap::unlock() {
	if (this->isValid())
	{
		SDL_UnlockSurface(this->surface);
	}
	this->lock_flag = false;
}

bool OOSDL::Bitmap::isLocked() const {
	return this->lock_flag;
}

SDL_Surface* OOSDL::Bitmap::createSurface(int flags, int width, int height)
{
	return SDL_CreateRGBSurface(flags, width, height, DEEPTH, RMASK, GMASK, BMASK, AMASK);
}

void OOSDL::Bitmap::set_SDL_Surface(SDL_Surface* newSurface, const Color* background_color)
{
	// Trata o novo surface.
	if (this->get_SDL_Surface() != NULL)
	{
		SDL_FreeSurface(this->get_SDL_Surface());
	}
	this->surface = newSurface;
	
	// Trata a cor de fundo.
	// É feito desta forma para evitar nova alocação dinâmica desnecessária caso já haja espaço alocado.
	     if (background_color != NULL && this->background_color == NULL)
	{
		this->background_color = new Color(*background_color);
	}
	else if (background_color != NULL && this->background_color != NULL)
	{
		(*this->background_color) = (*background_color);
	}
	else if (background_color == NULL && this->background_color != NULL)
	{
		delete this->background_color;
		this->background_color = NULL;
	}
	
	// Destrava o surface, por precaução. (para evitar estar lidando com um surface travado sem a flag estar corretamente setada)
	this->unlock();
}

OOSDL::Bitmap& OOSDL::Bitmap::operator=(const Bitmap& rhs)
{
	if (&rhs != this)
	{
		this->dispose();
		this->set_SDL_Surface( this->createSurface(0, rhs.getWidth(), rhs.getHeight()) );
		if (rhs.background_color != NULL)
		{
			this->background_color = new Color(*rhs.background_color);
		}
		this->lock_flag = false;
		this->drawBitmap(rhs);
	}
	return (*this);
}

void OOSDL::Bitmap::drawBitmap(const Bitmap& bmp, int x, int y)
{
	return this->drawBitmap(bmp, Rect(this->getWidth() - x, this->getHeight() - y, x, y));
}

void OOSDL::Bitmap::drawBitmap(const Bitmap& bmp, const Rect& dest)
{
	return this->drawBitmap(bmp, bmp.asRect(), dest);
}

void OOSDL::Bitmap::drawBitmap(const Bitmap& bmp, const Rect& orig, const Rect& dest) {
	// Traduz os Rects para formato SDL.
	SDL_Rect orig_rect = orig.to_SDL_Rect();
	SDL_Rect dest_rect = dest.to_SDL_Rect();
	// Realiza a cópia dos bitmaps.
	int error = SDL_BlitSurface(bmp.get_SDL_Surface(), &orig_rect, this->get_SDL_Surface(), &dest_rect);
	// Tratamento de erros.
	if (error < 0)
	{
		char* error = SDL_GetError(); // TODO
	}
}
/*
TTF_Font* OOSDL::Bitmap::getFont(const std::string& fontname, int fontsize)
throw (invalid_font)
{
	// Declaração de variáveis locais.
	TTF_Font* result = NULL;
	// Recupera o cache específico para este tamanho de fonte.
	std::map<int,TTF_Font*>& cache = OOSDL::Bitmap::fontCache[fontname];
	// Se não houver um objeto em cache, cria um para ser retornado,
	if (cache.find(fontsize) == cache.end())
	{
		cache[fontsize] = TTF_OpenFont(fontname.c_str(), fontsize);
	}
	// Valida a fonte antes de retorná-la.
	if ((result = cache[fontsize]) == NULL)
	{
		// Caso não seja, levanta uma exceção.
		throw invalid_font(fontname);
	}
	// Retorna a fonte obtida em cache.
	return result;
}

void OOSDL::Bitmap::drawText(int x, int y, const std::string& text, int fontsize, const Color& fontcolor, const std::string& fontname)
throw (invalid_font)
{
	// Recupera a fonte do cache. (uma nova é criada no cache e retornada se não existir)
	TTF_Font* font = getFont(fontname, fontsize);
	// TODO Os parâmetros deveriam setar o estilo da fonte(itálico, negrito, etc.), que deveria ser setado no objeto font(TTF_Font*) aqui utilizando as funções TTF.
	// Cria o bitmap à ser desenhado na tela.
	SDL_Surface* text_surface = TTF_RenderMethod(font, text.c_str(), fontcolor.to_SDL_Color());
	Bitmap text_bmp(text_surface);	// TODO Testar se não é NULL.
	// Desenha o bitmap na posição especificada.
	this->drawBitmap(text_bmp, x, y);
}
*/

void OOSDL::Bitmap::drawText(int x, int y, const std::string& text, const Font& font)
{
	// Recupera a fonte do cache. (uma nova é criada no cache e retornada se não existir)
	TTF_Font* ttf_font = const_cast<TTF_Font*>(font.to_TTF_Font()); // Gambiarra porque o método TTF_RenderMethod só aceita TTF_Font* não-constantes.
	// Cria o bitmap à ser desenhado na tela.
	SDL_Surface* text_surface = TTF_RenderMethod(ttf_font, text.c_str(), font.getColor().to_SDL_Color());
	Bitmap text_bmp(text_surface);	// TODO Testar se não é NULL.
	// Desenha o bitmap na posição especificada.
	this->drawBitmap(text_bmp, x, y);
}

void OOSDL::Bitmap::drawRect(const Rect& rect, const Color& contents, const Color& border, int border_boldness)
{
	this->drawRect(rect, border);
	// O compilador otimiza melhor assim do que chamando cada método individualmente.
	Rect contentsRect(
			rect.getWidth()  - border_boldness * 2,
			rect.getHeight() - border_boldness * 2,
			rect.getX() + border_boldness,
			rect.getY() + border_boldness
		);
	this->drawRect(contentsRect, contents);
}

void OOSDL::Bitmap::drawRect(const Rect& rect, const Color& color)
{
	int error;
	
	// Se este Bitmap for válido, pinta.
	if (this->isValid())
	{
		SDL_Surface* surface = this->get_SDL_Surface();
		SDL_Rect sdl_rect = rect.to_SDL_Rect();
		error = SDL_FillRect(surface, &sdl_rect, color.to_SDL_Uint32());
	}
	
	// Se não, declara um erro.
	else
	{
		error = -1;
	}
	
	// Tratamento de erros.
	if (error < 0)
	{
		sdl_error e; // TODO
	}
}

OOSDL::Color OOSDL::Bitmap::getPixel(int x, int y)
throw (std::invalid_argument)
{
	// Valida a coordenada.
	if (!Rect(0, 0, x, y).isInside(this->getRect()))
	{
		throw std::invalid_argument(INVALID_COORDINATE);
	}
	// Tranca este bitmap para leitura de pixels.
	this->lock();
	// Recupera o surface SDL para ler os pixels.
	SDL_Surface* surface = this->get_SDL_Surface();
	// Ponteiro para o pixel correto.
	Uint32* pixel = (Uint32*) surface->pixels + y * surface->pitch + x * OOSDL_BITMAP_BYTESPERPIXEL;	// TODO Deveria ler quantos bytes existem num pixel. Pois apesar de existir um padrão, ele é mutável.
	// Cria o objeto color baseado na cor do pixel e retorna.
	return Color(*pixel);
}

void OOSDL::Bitmap::setPixel(int x, int y, const Color& color)
throw (std::invalid_argument)
{
	// Valida a coordenada.
	if (!Rect(0, 0, x, y).isInside(this->getRect()))
	{
		throw std::invalid_argument(INVALID_COORDINATE);
	}
	// Tranca este bitmap para leitura de pixels.
	this->lock();
	// Recupera o surface SDL para ler os pixels.
	SDL_Surface* surface = this->get_SDL_Surface();
	// Ponteiro para o pixel correto.
	Uint32* pixel = (Uint32*) surface->pixels + y * surface->pitch + x * OOSDL_BITMAP_BYTESPERPIXEL;	// TODO Deveria ler quantos bytes existem num pixel. Pois apesar de existir um padrão, ele é mutável.
	// Muda o valor do ponteiro.
	*pixel = color.to_SDL_Uint32();
}

OOSDL::Rect OOSDL::Bitmap::getRect() const
{
	return this->asRect();
}

OOSDL::Rect OOSDL::Bitmap::asRect() const
{
	return Rect(this->getWidth(), this->getHeight(), 0, 0);
}

int OOSDL::Bitmap::getWidth() const {
	int result;
	if (this->surface != NULL) {
		result = this->surface->clip_rect.w;
	}
	else {
		result = 0;
	}
	return result;
}

int OOSDL::Bitmap::getHeight() const {
	int result;
	if (this->surface != NULL) {
		result = this->surface->clip_rect.h;
	}
	else {
		result = 0;
	}
	return result;
}

void OOSDL::Bitmap::setBackgroundColor(const Color& color) {
	// Caso uma cor já exista, é usado seu operador de atribuição para evitar realocação desnecessária.
	if (this->background_color == NULL)
	{
		this->background_color = new Color(color);
	}
	else
	{
		(*this->background_color) = color;
	}
}

SDL_Surface* OOSDL::Bitmap::get_SDL_Surface() {
	if (this->isLocked())
	{
		this->unlock();
	}
	return this->surface;
}

/**
 * Nota:
 * - Esta função quebra a constância lógica, mas é privada e usada internamente
 * 		para recuperar ponteiros para this->surface destrancados automaticamente.
 */
SDL_Surface* OOSDL::Bitmap::get_SDL_Surface() const
{
	Bitmap* unconstant_this = const_cast<Bitmap*>(this);
	return unconstant_this->get_SDL_Surface();
}

bool OOSDL::Bitmap::isValid() const
{
	return (this->surface != NULL);
}

void OOSDL::Bitmap::clear()
{
	this->clear(this->asRect());
}

void OOSDL::Bitmap::clear(const Rect& rect)
{
	if (this->background_color != NULL)
	{
		this->drawRect(rect, *this->background_color);
	}
	else
	{
		this->drawRect(rect, DEFAULT_COLOR);
	}
}

#endif
