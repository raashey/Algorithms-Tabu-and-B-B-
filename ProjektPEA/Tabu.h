#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TabuList.h"
#include "swapNeighbours.h"

using namespace std;
#pragma once
class Tabu
{
public:

	TabuList* tabuList;
	int ** costMatrix;

	int * currSolution;
	int numberOfIterations;
	int problemSize;
	int bestCostEver;
	int * bestSolutionEver;
	int bestCost;

	Tabu();
	Tabu(string name, int iterations);
	~Tabu();
	void readMatrixFromFile(string name);
	int calculateDistance(int* solution);
	void setupCurrentSolution();
	void printSolution(int* solution);
	void setupBestSolutionEver();
	void invoke();
	void swap(int a, int b);
	swapNeighbours bestSwapValue();
	void diversification();
};

