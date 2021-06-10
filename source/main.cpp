#include <SFML/Graphics.hpp>
#include "TextureManager.h"

int main()
{
    sf::Sprite mine(TextureManager::GetTexture(mine));

    return 0;
}
