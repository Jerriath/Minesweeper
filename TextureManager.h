#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class TextureManager
{
private:
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string textureName); //Going to type in LoadTexture("space") and that should work
public:
	static sf::Texture& GetTexture(string textureName);
	static void Clear(); //Call this once at the end of main to clear everything
};