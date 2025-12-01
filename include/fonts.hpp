#ifndef FONTS_HPP
#define FONTS_HPP   
#include <SFML/Graphics.hpp>
#include <string>   
using namespace sf;
class controlTexto {
public:
    bool loadFont(const std::string& fontPath)
    {
        if(!font.loadFromFile(fontPath))
        {
            return false;
        }
        return true;
    }    

    const Font& getFont() const
    {
        return font;
    }
    
private:
    Font font;
};
#endif