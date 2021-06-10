#pragma once
#include "TextureManager.h"
#include "Random.h"
#include "Board.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;


int main()
{
    Board newBoard;
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
            newBoard.SetWidth(widthInt);
            newBoard.SetHeight(heightInt);
            newBoard.SetMines(mineInt);
            newBoard.SetTiles(widthInt * heightInt);
        }
        file.close();
    } //Reads in the cfg file and uses the values in the file to create a board object and a window object both with correct sizes
    else
    {
        cout << "Error: cfg file was not opened properly!" << endl;
    }



	sf::RenderWindow window(sf::VideoMode(newBoard.Width() * 32, newBoard.Height() * 32 + 88), "Test Window");
    newBoard.MakeTileArray();
    newBoard.SetRandomMines();
    newBoard.FindAdjTilePtrs();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) //Checks to see if a tile was clicked
                {
                    int xPos = (int) sf::Mouse::getPosition(window).x;
                    int yPos = (int) sf::Mouse::getPosition(window).y;
                    xPos = xPos / 32;
                    yPos = yPos / 32;
                    if (xPos >= 0 && xPos < newBoard.Width() && yPos >= 0 && yPos < newBoard.Height())
                    {
                        if (newBoard.DidIWin())
                        {
                            break;
                        }
                        if (!newBoard.tileArray[yPos][xPos]->HasFlag() && !newBoard.MineFound())
                            newBoard.tileArray[yPos][xPos]->Reveal();
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) //Checks to see if a flag was placed
                {
                    int xPos = (int)sf::Mouse::getPosition(window).x;
                    int yPos = (int)sf::Mouse::getPosition(window).y;
                    xPos = xPos / 32;
                    yPos = yPos / 32;
                    if (xPos >= 0 && xPos < newBoard.Width() && yPos >= 0 && yPos < newBoard.Height())
                    {
                        if (newBoard.DidIWin() || newBoard.tileArray[yPos][xPos]->IsRevealed())
                        {
                            break;
                        }
                        newBoard.tileArray[yPos][xPos]->SetFlag();
                        if (newBoard.tileArray[yPos][xPos]->HasFlag())
                        {
                            newBoard.AddFlag();
                        }
                        else
                        {
                            newBoard.MinusFlag();
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Left) //Checks to see if the smiley was clicked
                {
                    int xPos = (int)sf::Mouse::getPosition(window).x;
                    int yPos = (int)sf::Mouse::getPosition(window).y;
                    int width = newBoard.Width();
                    int height = newBoard.Height();
                    int buttonX = (width*32 - 64) / 2;
                    int buttonY = (height * 32);
                    if (xPos >= buttonX && xPos < buttonX + 64 && yPos >= buttonY && yPos < buttonY + 64)
                    {
                        newBoard.RemakeTileArray();
                        newBoard.SetRandomMines();
                        newBoard.FindAdjTilePtrs();
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Left) //Checks to see if a test board or debug was clicked
                {
                    int xPos = (int)sf::Mouse::getPosition(window).x;
                    int yPos = (int)sf::Mouse::getPosition(window).y;
                    int width = newBoard.Width();
                    int height = newBoard.Height();
                    int buttonX = (width * 32 - 64) / 2;
                    buttonX += 128; //See the DrawTestBoard() function to see why
                    int buttonY = (height * 32);
                    if (xPos >= buttonX && xPos < buttonX + 64 && yPos >= buttonY && yPos < buttonY + 64)
                    {
                        if (newBoard.DidIWin())
                        {
                            break;
                        }
                        newBoard.AlterDebugState();
                        newBoard.DrawDebugMines(window);//////////// Should be a function that shows all mines here
                    }
                    buttonX += 64; //See the DrawTestBoard() function to see why
                    if (xPos >= buttonX && xPos < buttonX + 64 && yPos >= buttonY && yPos < buttonY + 64)
                    {
                        newBoard.ReadBoard("boards/testboard1.brd");
                    }
                    buttonX += 64; //See the DrawTestBoard() function to see why
                    if (xPos >= buttonX && xPos < buttonX + 64 && yPos >= buttonY && yPos < buttonY + 64)
                    {
                        newBoard.ReadBoard("boards/testboard2.brd");
                    }
                    buttonX += 64; //See the DrawTestBoard() function to see why
                    if (xPos >= buttonX && xPos < buttonX + 64 && yPos >= buttonY && yPos < buttonY + 64)
                    {
                        newBoard.ReadBoard("boards/testboard3.brd");
                    }
                }
            }
        }

        window.clear();
        newBoard.PrepBoard(window);
        window.display();
    }

	return 0;
}