#pragma once
#include <vector>
class Node
{

public:


	int numberOfCities;
	int **costMatrix;
	std::vector <int> path;

	int lowerBound;
	int name;


	Node();
	Node(int **costMatrix, int numberOFcities, Node *n, int name);

	void minAndSubtract();
	bool endPath();
	std::vector <Node*> *generateChildren();
	~Node();
};

