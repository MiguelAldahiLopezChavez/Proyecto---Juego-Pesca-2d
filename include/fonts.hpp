// fonts.hpp
// Clase auxiliar para cargar y exponer una `Font` de SFML.
// Permite acceder a la fuente cargada desde otras partes del juego.
#ifndef FONTS_HPP
#define FONTS_HPP   
#include <SFML/Graphics.hpp>
#include <string>   
using namespace sf;
// Control simple de texto/ fuente: carga y acceso seguro a `Font`.
class controlTexto {
public:
    // Intenta cargar la fuente desde `fontPath`. Devuelve true si tiene éxito.
    bool loadFont(const std::string& fontPath)
    {
        if(!font.loadFromFile(fontPath))
        {
            return false;
        }
        return true;
    }    

    // Devuelve la fuente cargada (referencia const).
    const Font& getFont() const
    {
        return font;
    }
    
private:
    // Recurso `Font` utilizado por los textos del juego.
    Font font;
};
#endif