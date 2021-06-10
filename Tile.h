#pragma once
#include "TextureManager.h"
#include <vector>

class Tile
{
	int row;
	int col;
	bool hasMine;
	bool hasFlag;
	bool isRevealed;
	vector<Tile*> adjTiles;
	int adjMines;
	sf::Sprite frontTile;
	sf::Sprite backTile;
	sf::Sprite flag;
public:
	Tile();
	Tile(int row, int col);

	//accessors
	sf::Sprite FrontTile();
	sf::Sprite BackTile();
	sf::Sprite Flag();
	int AdjMines();
	bool IsRevealed();
	bool HasMine();
	bool HasFlag();
	vector<Tile*>& AdjTiles();

	//mutator
	void SetFrontTile(sf::Sprite newSprite); //Changes the front tile to another sprite (i.e. mine or number)
	void SetMine(); //just sets the hasMine attribute to true
	void SetAdjMines(int mines); //sets adjMines attribute to input value (input value is found in FindAdjTilePtrs funct in Board.cpp)
	void Reveal(); //Reveal the tile; if there is a mine, a mine will show and all other mines will be revealed; if it is a number, it's number will show
	void SetAdjTiles(vector<Tile*> adjTiles);
	void RecursiveReveal();
	void SetFlag();
};