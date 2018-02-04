#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
#include "Node.h"

class TSP
{
private:

	int numberOFcities;
	int **costMatrix;
	std::vector<std::string> vect;
	std::vector<int> x;
	std::vector<int> y;
	Node* endNode;

public:

	TSP();
	TSP(std::string);
	TSP(int size);

	void print();
	void printResult();
	void randomMatrix(int);
	void readMatrixFromFile(std::string);
	void BB();

	~TSP();
};