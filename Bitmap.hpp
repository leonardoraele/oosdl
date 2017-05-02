#ifndef OOSDL_Bitmap_HPP
#define OOSDL_Bitmap_HPP

/**
 * Notas:
 * - Antes de criar um objeto desta classe, é necessário criar um objeto Screen.
 * - Criar dois objetos Bitmap que gerencial um mesmo objeto SDL pode levar à comportamento indefinido e é altamente desencorajado.
 * TODO:
 * - Implementar a parte referente ao background_color.
 */
 
#define DEFAULT_COLOR Color(0, 0, 0, 0)

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <exception>
#include <map>
#include "TTFClient.hpp"
#include "invalid_font.cpp"
#include "sdl_error.cpp"
#include "Color.cpp"
#include "Rect.cpp"
#include "Font.cpp"

namespace OOSDL {
	class Bitmap : public TTFClient {
		SDL_Surface* surface;
		Color* background_color;
		
		// Atributos/métodos de controle ao bloqueio de acesso direto aos pixels:
		bool lock_flag;
		void lock();			
		void unlock();
		bool isLocked() const;
		
		SDL_Surface* get_SDL_Surface() const;												// Versão constante do método to_SDL_Surface. Usado internamente para recuperar o surface já destrancado de objetos Bitmaps constantes.
		static SDL_Surface* createSurface(int flags, int width, int height);				// Aloca espaço para um novo surface em branco chamando SDL_CreateRGBSurface.
//		TTF_Font* getFont(const std::string& fontname, int fontsize)						// Recupera a fonte com determinadas propriedades do cache.
//				throw (invalid_font);
//		static std::map< std::string, std::map< int, TTF_Font* > > fontCache;				// Tabela: para cada fonte e tamanho de fonte existe um objeto TTF_Font.
	protected:
		virtual void dispose();																// Libera os recursos alocados pelo Bitmap.
		virtual void set_SDL_Surface(SDL_Surface*, const Color* = NULL);					// Esta função não libera a surface antiga. Para evitar vazamento de memória, antes de usar estar função deve-se recuperar a surface antigo utilizando get_SDL_Surface e liberá-lo utilizando SDL_FreeSurface.
	public:
		// Construtores.
		Bitmap(int width, int height, const Color& bitmap_color = DEFAULT_COLOR)			// Cria um bitmap inicialmente em branco ou completamente preenchido por uma cor.
				throw (sdl_error);
		Bitmap(const std::string& filename, const Color& background_color = DEFAULT_COLOR)	// Cria um bitmap que carrega um arquivo de imagem.
				throw (sdl_error);
		Bitmap(SDL_Surface*);																// Cria um bitmap que gerencia um surface SDL.
		Bitmap(const Bitmap&)																// Cria um bitmap com um novo surface SDL baseado no surface SDL de outro bitmap.
				throw (sdl_error);
		virtual ~Bitmap();
		
		// Operadores.
		virtual Bitmap& operator=(const Bitmap&);											// O operador de atribuição irá criar um novo surface SDL com base no surface do outro bitmap.
		
		// Cópia de bitmaps.
		virtual void drawBitmap(const Bitmap& bmp, int x = 0, int y = 0);					// Copia o bitmap bmp para este bitmap nas posições especificadas.
		virtual void drawBitmap(const Bitmap& bmp, const Rect& dest);						// Copia o bitmap bmp para o retângulo dest deste bitmap. (equivalente à "drawBitmap(bmp, bmp.getRect(), dest)")
		virtual void drawBitmap(const Bitmap& bmp, const Rect& orig, const Rect& dest);		// Copia o retângulo orig do bitmap bmp para o retângulo dest deste bitmap.
		
		// Escrita de texto.
		virtual void drawText(int x, int y, const std::string& text, const Font& font = Font::getDefaultFont());									// Escreve um determinado texto na posição (x,y) especificada com a fonte especificada.
//		virtual void drawText(const Rect&, const std::string& text, int fontsize, int align, const Color& fontcolor, const std::string& fontname)	// Escreve um texto da fonte e cor desejados que só pode aparecer dentro do retâgulo rect e que estará devidamente alinhado de acordo com o parâmetro align.
//				throw (invalid_font);																												// Joga uma exceção se a fonte especificada não for válida.
//		virtual std::ostringstream createTextDrawer(int x, int y, const Font& font = Font::getDefaultFont());											// Cria um output stream para facilitar escrita na tela utilizando internamente o método drawText com o texto recebido pelo output stream.
		
		// Desenho manual.
//		virtual void drawLine(int ax, int ay, int bx, int by, const Color& color = Color(0,0,0), int boldness = 1);									// Desenha uma linha que vai do ponto (ax,ay) ao ponto (bx,by) da cor color. Quanto maior o atributo boldness, mais groça a linha é desenhada.
//		virtual void drawCircle(int x, int y, int r, const Color& color);																			// Desenha um círculo de centro (x,y) de raio r da cor color.
//		virtual void drawCircle(int x, int y, int r, const Color& contents, const Color& border, int border_boldness);								// Desenha um círculo de centro (x,y) de raio r da cor contents com uma borda de cor border. quanto maior o border_boldness, maior a borda do círculo.
		virtual void drawRect(const Rect&, const Color&);																							// Desenha um retângulo neste bitmap da cor especificada.
		virtual void drawRect(const Rect&, const Color& contents, const Color& border, int border_boldness);										// Desenha um retângulo neste bitmap com as cores de borda e interna específicas. border_boldness é o tamanho da borda.
//		virtual void drawPoligon(const std::pair<int,int>* points, size_t size, const Color&);														// Desenha um poligono da cor desejada. O vetor de std::pair<int,int> define os pontos que representam os vértices do poligono.
//		virtual void drawPoligon(const std::pair<int,int>* points, size_t size, const Color& contents, const Color& border, int border_boldness);	// Desenha um poligono da cor de conteúdo e borda desejados. O vetor de std::pair<int,int> define os pontos que representam os vértices do poligono.
		
		// Leitura do bitmap.
		virtual Color getPixel(int x, int y)								// Recupera a cor de um determinado pixel. (não é constante porque é necessário travar o surface SDL para poder ler os pixels, logo, o recurso é alterado)
				throw (std::invalid_argument);								// Joga um erro se as cordenadas forem inválidas.
		virtual void setPixel(int x, int y, const Color&)					// Altera a cor de um determinado pixel.
				throw (std::invalid_argument);								// Joga um erro se as cordenadas forem inválidas.
		
		// Operações de tamanho do bitmap.
		virtual Rect getRect() const;										// Cria um retângulo de início (0,0) que possui o tamanho do bitmap.
		virtual Rect asRect() const;										// Equivalente à getRect. O método é duplicado(este chama o outro internamente) por coveniência de nomenclatura.
		virtual int getWidth() const;										// Verifica o tamanho horizontal do bitmap.
		virtual int getHeight() const;										// Verifica o tamanho vertical do bitmap.
		
		// Transparência.
		virtual void setBackgroundColor(const Color&);						// Altera a cor que é usada como transparência do bitmap.
		virtual void clear();												// Limpa todo o conteúdo deste bitmap o preenchendo com uma cor padrão. Equivalente à clear(bitmap.asRect()).
		virtual void clear(const Rect&);									// Limpa uma porção retangular especificada deste bitmap a preenchendo com uma cor padrão.
		
		// Métodos SDL.
		virtual SDL_Surface* get_SDL_Surface();								// Recupera o surface. (não é const porque retorna um ponteiro de objeto não constante)
		/*TODO*/// virtual const SDL_Surface* get_SDL_Surface() const;
		virtual bool isValid() const;										// Verifica se o bitmap é atualmente válido. (surface != NULL)
	};
};
#endif
