#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>
#include "Array2D.hpp"
int findLiveNeighbors(Array2D<std::string> grid, int x, int y)
{
	int liveNeighbours = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int atY = y + j;
			int atX = x + i;
			if (atX >= grid.getRows() || atY >= grid.getCols() || atX < 0 || atY < 0)
			{
				continue;
			}
			std::string elemAtGrid = grid.getElem(atX, atY).c_str();
			if (elemAtGrid == "x")
			{
				liveNeighbours += 1;
			}
		}
	}
	//extract value stored at the central grid
	std::string elemAt = grid.getElem(x, y).c_str();

	return elemAt != "x" ? liveNeighbours : liveNeighbours - 1;
}
//applies rules of life
Array2D<std::string> getNextGen(Array2D<std::string> curGen)
{
	Array2D<std::string> nextGen(curGen.getRows(), curGen.getCols());
	nextGen.setArr("-");
	for (int i = 0; i < curGen.getRows(); i++)
	{
		for (int j = 0; j < curGen.getCols(); j++)
		{
			int liveNeighbours = findLiveNeighbors(curGen, i, j);
			std::string currElem = curGen.getElem(i, j);
			//If cell is alive and has 0 or 1 live neighbors
			if (currElem == "x" && liveNeighbours < 2)
			{
				nextGen.setElem("-", i, j);
			}
			//A stable cell has exactly 2 live neighbors
			if (liveNeighbours == 2)
			{
				nextGen.setElem(currElem, i, j);
			}
			//If a cell has 3 live neighbors and is unoccupied
			if (liveNeighbours == 3 && currElem == "-")
			{
				nextGen.setElem("x", i, j);
			}
			//Overcrowding
			if (liveNeighbours >= 4 && currElem == "x")
			{
				nextGen.setElem("-", i, j);
			}
		}
	}
	return nextGen;
}
void printGeneration(Array2D<std::string> generation)
{
	for (int i = 0; i < generation.getCols(); i++)
	{
		for (int j = 0; j < generation.getRows(); j++)
		{
			std::cout << generation.getElem(j, i);
		}
		std::cout << "\n";
	}
}
Array2D<std::string> generateRandom()
{
	srand(time(NULL));
	int rows = 1 + (rand() % 40), cols = 1 + (rand() % 40);
	Array2D<std::string> generation(rows, cols);
	for (int i = 0; i < generation.getCols(); i++)
	{
		for (int j = 0; j < generation.getRows(); j++)
		{
			int randomNum = 1 + (rand() % 2);
			std::string element;
			if (randomNum == 1)
			{
				element = "-";
			}
			else
			{
				element = "x";
			}
			generation.setElem(element, j, i);
		}
	}
	return generation;
}
// ADD
Array2D<std::string> readFromFile(std::string filename)
{
	std::ifstream infile;
	std::string element;
	std::string r, c;
	int rows, cols;
	infile.open(filename);
	infile >> r;
	infile >> c;
	Array2D<std::string> generation(std::stoi(r), std::stoi(c));
	generation.setArr("-");
	for (int i = 0; i < generation.getCols(); i++)
	{
		for (int j = 0; j < generation.getRows(); j++)
		{
			infile >> element;
			generation.setElem(element, j, i);
		}
	}

	infile.close();
	return generation;
}
bool isStable(Array2D<std::string> curGeneration, Array2D<std::string> nextGeneration)
{
	int countEqual = 0;
	for (int col = 0; col < curGeneration.getCols(); col++)
	{
		for (int row = 0; row < curGeneration.getRows(); row++)
		{
			if (curGeneration.getElem(row, col) != nextGeneration.getElem(row, col))
			{
				return false;
			}
		}
	}
	std::cout << "The system is stable\n";
	return true;
}

void controlledSimulation(Array2D<std::string> curGeneration)
{
	Array2D<std::string> nextGeneration = getNextGen(curGeneration);
	//std::string keyPressed = "N";
	while (!isStable(curGeneration, nextGeneration))
	{
		system("clear");
		curGeneration = nextGeneration;
		nextGeneration = getNextGen(curGeneration);
		printGeneration(nextGeneration);
		std::cin.get();
		std::cout << "Press enter to continue\n";
	}
}
void timedSimulation(Array2D<std::string> curGeneration, int t)
{
	std::chrono::seconds duration(t);
	Array2D<std::string> nextGeneration = getNextGen(curGeneration);
	while (!isStable(curGeneration, nextGeneration))
	{
		system("clear");
		curGeneration = nextGeneration;
		nextGeneration = getNextGen(curGeneration);
		printGeneration(nextGeneration);

		std::this_thread::sleep_for(duration);
	}
}
Array2D<std::string> initialGeneration()
{
	std::string options[3] = { "Read from file", "Generate random", "Exit" };

	int choice;
	for (int i = 0; i < 3; i++)
	{
		std::cout << i + 1 << ". " << options[i] << std::endl;
	}
	std::cout << "Your choice: ";
	std::cin >> choice;
	switch (choice)
	{
	case 1:
		return readFromFile("config.txt");
	case 2:
		return generateRandom();
	default:
		std::cout << "Invalid entry\n";
		std::exit(1);
	}
}
int main()
{

	Array2D<std::string> curGeneration = initialGeneration();
	std::string options[4] = { "New configuration","Timed Simulation", "Controlled simulation", "Exit" };
	int choice;
	while (1)
	{
		std::cout << "\t\tMAIN MENU" << std::endl;
		for (int i = 0; i < 3; i++)
		{
			std::cout << i + 1 << ". " << options[i] << std::endl;
		}
		std::cout << "Your choice: ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			curGeneration = initialGeneration();
			break;
		case 2:
			int t;
			std::cout << "Enter delay time in seconds:";
			std::cin >> t;
			timedSimulation(curGeneration, t);
			break;
		case 3:
			controlledSimulation(curGeneration);
			break;
		case 4:
			std::exit(0);
			break;
		}
	}
	return 0;
}
