#pragma once
#include "TextureManager.h"
#include <vector>
#include "Tile.h"

class Board
{
	int width;
	int height;
	int mineCount;
	int tileCount;
	int flagCount;
	bool mineFound;
	bool winCondition;
	bool debugState;
public:
	vector<vector<Tile*>> tileArray;
	Board();
	Board(int width, int height, int mineCount, int tileCount);
	void PrepBoard(sf::RenderWindow& window); //drawing out the board before displaying
	void ReadBoard(string fileName); //reads in a test board and displays the new board in the window
	void MakeTileArray(); //creates tile array with correct number of tiles given by cfg file
	void RemakeTileArray();
	void CheckMines(); //checks to see if a mine was clicked; reveals all other mines if so
	void SetRandomMines(); //sets number of mines (specified in cfg file) on the board in random locations
	void FindAdjTilePtrs(); //makes vector containing all adj tiles ptrs; if an adj space is out of bounds, a nullptr is used in vector
	void DrawSmiley(sf::RenderWindow& window);
	void DrawTestBoards(sf::RenderWindow& window);
	void DrawCounter(sf::RenderWindow& window);
	void DrawDebug(sf::RenderWindow& window);
	void DrawDebugMines(sf::RenderWindow& window);

	//Mutators
	void SetWidth(int width);
	void SetHeight(int height);
	void SetMines(int mines);
	void SetTiles(int tiles);
	void CheckWinCondition();
	void AddFlag();
	void MinusFlag();
	void AlterDebugState();

	//Accessors
	int Width();
	int Height();
	int Mines();
	int Tiles();
	int Flags();
	bool MineFound();
	bool DidIWin();
};