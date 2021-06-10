#pragma once
#include "Board.h"
#include "Random.h"
#include <fstream>
#include <algorithm>

Board::Board()
{
	this->width = 0;
	this->height = 0;
	this->mineCount = 0;
	this->tileCount = 0;
	this->flagCount = 0;
	this->mineFound = false;
	this->winCondition = false;
	this->debugState = false;
}
Board::Board(int width, int height, int mineCount, int tileCount)
{
	this->width = width;
	this->height = height;
	this->mineCount = mineCount;
	this->tileCount = tileCount;
	this->flagCount = 0;
	this->mineFound = false;
	this->winCondition = false;
	this->debugState = false;
}
void Board::PrepBoard(sf::RenderWindow& window)
{
	CheckMines();
	CheckWinCondition();
	for (int i = 0; i < Height(); i++) //This loop is dynamic and will alter itself for any cfg file
	{
		for (int j = 0; j < Width(); j++)
		{
			sf::Sprite background = tileArray[i][j]->BackTile();
			background.setPosition((j * 32.f), (i * 32.f));
			window.draw(background);
			if (tileArray[i][j]->AdjMines() == 0)
			{
				if (tileArray[i][j]->IsRevealed() == true && tileArray[i][j]->HasMine() == false)
				{
					continue;
				}
			}
			if (mineFound && tileArray[i][j]->HasFlag())
			{
				sf::Sprite flag = tileArray[i][j]->Flag();
				flag.setPosition((j * 32.f), (i * 32.f));
				window.draw(flag);
			}
			sf::Sprite foreground = tileArray[i][j]->FrontTile();
			foreground.setPosition((j * 32.f), (i * 32.f));
			window.draw(foreground);
			if (tileArray[i][j]->HasFlag() && mineFound == false)
			{
				sf::Sprite flag = tileArray[i][j]->Flag();
				flag.setPosition((j * 32.f), (i * 32.f));
				window.draw(flag);
			}
		}
	}
	//This section is for drawing the buttons and counter

	DrawSmiley(window);
	DrawTestBoards(window);
	DrawCounter(window);
	DrawDebug(window);

	if (debugState)
	{
		DrawDebugMines(window);
	}
}
void Board::ReadBoard(string fileName) //takes in a filename (brd file) and alters the board to match the new board layout
{
	ifstream file;
	vector<string> boardData;
	file.open(fileName);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			boardData.push_back(line);
		}
		file.close();
	}
	//This is the start of actuallying modifying the map displaying in the window

	int minesCounted = 0;
	for (int i = 0; i < Height(); i++) //This loop is dynamic and will alter itself for any cfg file
	{
		for (int j = 0; j < Width(); j++)
		{
			Tile* newTile = new Tile(i, j);
			delete tileArray[i][j];
			tileArray[i][j] = newTile;
			if (boardData[i][j] == '1')
			{
				tileArray[i][j]->SetMine();
				minesCounted++;
			}

		}

	}
	FindAdjTilePtrs();
	mineCount = minesCounted;
	mineFound = false;
	flagCount = 0;
	winCondition = false;
}
void Board::MakeTileArray()
{
	for (int i = 0; i < Height(); i++) //This loop is dynamic and will alter itself for any cfg file
	{
		tileArray.push_back(vector<Tile*>());  //This pushes back the array of Tiles onto the 2d array
		for (int j = 0; j < Width(); j++)
		{
			Tile* currentTile = new Tile(i, j); //Creates a tile with the coresponding coordinates (i is the row (y) and i is the col (x)
			tileArray[i].push_back(currentTile);
		}
	}
}
void Board::RemakeTileArray()
{
	ifstream file;
	file.open("boards/config.cfg");
	if (file.is_open())
	{
		string width;
		string height;
		string numOfMines;
		while (getline(file, width))
		{
			getline(file, height);
			getline(file, numOfMines);
			int widthInt = stoi(width);
			int heightInt = stoi(height);
			int mineInt = stoi(numOfMines);
			SetWidth(widthInt);
			SetHeight(heightInt);
			SetMines(mineInt);
			SetTiles(widthInt * heightInt);
			mineFound = false;
			winCondition = false;
			debugState = false;
		}
		file.close();
	} //Rereads cfg to get correct info again
	else
	{
		cout << "Error: cfg file was not opened properly!" << endl;
	}
	flagCount = 0;
	for (int i = 0; i < Height(); i++) //This loop is dynamic and will alter itself for any cfg file
	{
		for (int j = 0; j < Width(); j++)
		{
			delete tileArray[i][j];
			Tile* currentTile = new Tile(i, j); //Creates a tile with the coresponding coordinates (i is the row (y) and i is the col (x)
			tileArray[i][j] = currentTile;
		}
	}
}
void Board::CheckMines()
{
	for (int i = 0; i < Height(); i++) //Loop checks to see if there is a mine that has already been revealed
	{
		for (int j = 0; j < Width(); j++)
		{
			if (tileArray[i][j]->IsRevealed())
			{
				if (tileArray[i][j]->HasMine())
				{
					mineFound = true;
					break;
				}
			}
		}
		if (mineFound)
		{
			break;
		}
	}
	if (mineFound)
	{
		for (int i = 0; i < Height(); i++) //Loop reveals all hidden mines if the first loop found that a mine was already revealed
		{
			for (int j = 0; j < Width(); j++)
			{
				if (tileArray[i][j]->HasMine() == true)
				{
					sf::Sprite mine(TextureManager::GetTexture("mine"));
					tileArray[i][j]->SetFrontTile(mine);
				}
			}
		}
	}
} 
void Board::SetRandomMines() //Sets mines in random locations depending on the num of mines specified in cfg file
{
	vector<int> randomNums;
	int value = -1;
	for (int i = 0; i < mineCount; i++)
	{
		do
		{
			value = Random::RandInt(0, tileCount - 1);
		} while (find(randomNums.begin(), randomNums.end(), value) != randomNums.end());
		randomNums.push_back(value);
	}
	for (int i = 0; i < mineCount; i++)
	{
		int tileNum = randomNums[i];
		int row = tileNum / Width();
		int col = tileNum % Width();
		this->tileArray[row][col]->SetMine();
	}
}
void Board::FindAdjTilePtrs()
{
	for (int h = 0; h < tileCount; h++)
	{
		vector<Tile*> adjTiles;
		int row = h / Width();
		int col = h % Width();
		int numOfMines = 0;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0 || i + row < 0 || i + row >= Height() || j + col < 0 || j + col >= Width())
					adjTiles.push_back(nullptr);
				else
				{
					adjTiles.push_back(tileArray[i + row][j + col]);
					if (tileArray[i + row][j + col]->HasMine())
						numOfMines++;
				}
			}
		}
		tileArray[row][col]->SetAdjTiles(adjTiles);
		tileArray[row][col]->SetAdjMines(numOfMines);
		adjTiles.clear();
		numOfMines = 0;
	}
}
void Board::DrawSmiley(sf::RenderWindow& window)
{
	if (winCondition) //should first check to see if all mines are flagged; need to make a function to check this and put it here
	{
		sf::Sprite smile(TextureManager::GetTexture("face_win"));
		int width = Width();
		int height = Height();
		int buttonX = (width * 32 - 64) / 2;
		int buttonY = (height * 32);
		smile.setPosition((float)buttonX, (float)buttonY);
		window.draw(smile);
	}
	else if (mineFound)
	{
		sf::Sprite smile(TextureManager::GetTexture("face_lose"));
		int width = Width();
		int height = Height();
		int buttonX = (width * 32 - 64) / 2;
		int buttonY = (height * 32);
		smile.setPosition((float)buttonX, (float)buttonY);
		window.draw(smile);
	}
	else if (!mineFound)
	{
		sf::Sprite smile(TextureManager::GetTexture("face_happy"));
		int width = Width();
		int height = Height();
		int buttonX = (width * 32 - 64) / 2;
		int buttonY = (height * 32);
		smile.setPosition((float)buttonX, (float)buttonY);
		window.draw(smile);
	}
}
void Board::CheckWinCondition()
{
	int correctFlags = 0;
	for (int i = 0; i < Height(); i++)
	{
		for (int j = 0; j < Width(); j++)
		{
			if (tileArray[i][j]->HasFlag())
			{
				if (tileArray[i][j]->HasMine())
				{
					correctFlags++;
				}
				else
				{
					correctFlags = -1;
					break;
				}
			}
			else if (!tileArray[i][j]->IsRevealed())
			{
				correctFlags = -1;
				break;
			}
		}
		if (correctFlags == -1)
			break;
	}
	if (correctFlags == mineCount)
	{
		winCondition = true;
	}
}
void Board::DrawTestBoards(sf::RenderWindow& window)
{
	int width = Width();
	int height = Height();
	int buttonX = (width * 32 - 64) / 2;
	int buttonY = (height * 32); //All buttons are at the same height
	buttonX += 192; //192 because the first testboard button is offset from the smiley by 64 pixels (plus the width of the smiley)
	sf::Sprite testOne(TextureManager::GetTexture("test_1"));
	testOne.setPosition((float)buttonX, (float)buttonY);
	buttonX += 64; //Second button offset by 64 pixels from first button
	sf::Sprite testTwo(TextureManager::GetTexture("test_2"));
	testTwo.setPosition((float)buttonX, (float)buttonY);
	buttonX += 64; //Third button offset by 64 pixels from first button
	sf::Sprite testThree(TextureManager::GetTexture("test_3"));
	testThree.setPosition((float)buttonX, (float)buttonY);
	window.draw(testOne);
	window.draw(testTwo);
	window.draw(testThree);
}
void Board::DrawCounter(sf::RenderWindow& window)
{
	sf::Sprite counter(TextureManager::GetTexture("digits"));
	string minesLeft = to_string(mineCount - flagCount);
	for (unsigned int i = 0; i < minesLeft.size(); i++)
	{
		if (minesLeft[i] == 45)
		{
			auto rectangle = sf::Rect<int>::Rect(210, 0, 21, 32);
			counter.setTextureRect(rectangle);
			counter.setPosition((float)21 + i * 21, (float)this->Height() * 32);
			window.draw(counter);
		}
		else
		{
			int currentDigit = (minesLeft[i] - '0');
			auto rectangle = sf::Rect<int>::Rect(currentDigit * 21, 0, 21, 32);
			counter.setTextureRect(rectangle);
			counter.setPosition((float)(21 + i * 21), (float)this->Height() * 32);
			window.draw(counter);
		}
	}
}
void Board::DrawDebug(sf::RenderWindow& window)
{
	int width = Width();
	int height = Height();
	int buttonX = (width * 32 - 64) / 2;
	int buttonY = (height * 32);
	buttonX += 128;
	sf::Sprite debug(TextureManager::GetTexture("debug"));
	debug.setPosition((float)buttonX, (float)buttonY);
	window.draw(debug);
}
void Board::DrawDebugMines(sf::RenderWindow& window)
{
	for (int i = 0; i < Height(); i++) //This loop is dynamic and will alter itself for any cfg file
	{
		for (int j = 0; j < Width(); j++)
		{
			if (tileArray[i][j]->HasMine())
			{
				sf::Sprite mine(TextureManager::GetTexture("mine"));
				mine.setPosition((j * 32.f), (i * 32.f));
				window.draw(mine);
			}
		}

	}
}

//Mutators
void Board::SetWidth(int width)
{
	this->width = width;
}
void Board::SetHeight(int height)
{
	this->height = height;
}
void Board::SetMines(int mines)
{
	this->mineCount = mines;
}
void Board::SetTiles(int tiles)
{
	this->tileCount = tiles;
}
void Board::AddFlag()
{
	this->flagCount++;
}
void Board::MinusFlag()
{
	this->flagCount--;
}
void Board::AlterDebugState()
{
	if (debugState)
		debugState = false;
	else
		debugState = true;
}

//Accessors
int Board::Width()
{
	return this->width;
}
int Board::Height()
{
	return this->height;
}
int Board::Mines()
{
	return this->mineCount;
}
int Board::Tiles()
{
	return this->tileCount;
}
int Board::Flags()
{
	return flagCount;
}
bool Board::MineFound()
{
	return mineFound;
}
bool Board::DidIWin()
{
	return winCondition;
}