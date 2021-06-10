#pragma once
#include "Tile.h"

Tile::Tile()
{
	this->row = 0;
	this->col = 0;
	this->hasMine = false;
	this->hasFlag = false;
	this->adjMines = 0;
	this->isRevealed = false;
	sf::Sprite front(TextureManager::GetTexture("tile_hidden"));
	this->frontTile = front;
	sf::Sprite back(TextureManager::GetTexture("tile_revealed"));
	this->backTile = back;
	sf::Sprite flag(TextureManager::GetTexture("flag"));
	this->flag = flag;
}
Tile::Tile(int row, int col)
{
	this->row = row;
	this->col = col;
	this->hasMine = false;
	this->hasFlag = false;
	this->adjMines = 0;
	this->isRevealed = false;
	sf::Sprite front(TextureManager::GetTexture("tile_hidden"));
	this->frontTile = front;
	sf::Sprite back(TextureManager::GetTexture("tile_revealed"));
	this->backTile = back;
	sf::Sprite flag(TextureManager::GetTexture("flag"));
	this->flag = flag;
}


//Accessors
sf::Sprite Tile::FrontTile()
{
	return this->frontTile;
}
sf::Sprite Tile::BackTile()
{
	return this->backTile;
}
sf::Sprite Tile::Flag()
{
	return flag;
}
int Tile::AdjMines()
{
	return adjMines;
}
bool Tile::IsRevealed()
{
	return isRevealed;
}
bool Tile::HasMine()
{
	return hasMine;
}
bool Tile::HasFlag()
{
	return hasFlag;
}
vector<Tile*>& Tile::AdjTiles()
{
	return adjTiles;
}

//Mutators
void Tile::SetFrontTile(sf::Sprite newSprite)
{
	this->frontTile = newSprite;
}
void Tile::SetMine()
{
	this->hasMine = true;
}
void Tile::SetAdjMines(int mines)
{
	this->adjMines = mines;
}
void Tile::Reveal()
{
	if (adjMines == 0 && HasMine() == false)
	{
		RecursiveReveal();
	}
	this->isRevealed = true;
	if (this->hasMine == true)
	{
		sf::Sprite front(TextureManager::GetTexture("mine"));
		this->frontTile = front;
	}
	else if (adjMines != 0)
	{
		sf::Sprite front(TextureManager::GetTexture("number_" + to_string(adjMines)));
		this->frontTile = front;
	}
}
void Tile::SetAdjTiles(vector<Tile*> adjTiles)
{
	this->adjTiles = adjTiles;
}
void Tile::RecursiveReveal()
{
	if (isRevealed)
	{
		return;
	}
	if (adjMines == 0 && HasMine() == false && HasFlag() == false)
	{
		this->isRevealed = true;
		for (unsigned int i = 0; i < AdjTiles().size(); i++)
		{
			vector<Tile*> currentList = this->AdjTiles();
			if (currentList[i] != nullptr)
				currentList[i]->RecursiveReveal();
		}
	}
	if (adjMines != 0 && HasMine() == false && HasFlag() == false)
	{
		this->isRevealed = true;
		sf::Sprite front(TextureManager::GetTexture("number_" + to_string(adjMines)));
		this->frontTile = front;
	}
}
void Tile::SetFlag()
{
	if (!isRevealed)
	{
		if (HasFlag())
		{
			hasFlag = false;
		}
		else if (!HasFlag())
		{
			hasFlag = true;
		}
	}
}