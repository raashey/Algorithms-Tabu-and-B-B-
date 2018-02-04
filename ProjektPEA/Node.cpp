#include "Node.h"
#include <climits>
#include <iostream>
Node::Node()
{
}


Node::~Node()
{
	for (int i = 0; i < numberOfCities; i++) {				//destruktor
		delete[] costMatrix[i];
	}
	delete[] costMatrix;
}

Node::Node(int **costMatrix, int numberOFcities, Node *n, int name)				//konstruktor parametrowy
{
	this->costMatrix = new int*[numberOFcities];
	for (int i = 0; i < numberOFcities; i++)
	{
		this->costMatrix[i] = new int[numberOFcities];
		for (int j = 0; j < numberOFcities; j++)
		{
			this->costMatrix[i][j] = costMatrix[i][j];
		}
	}
	this->numberOfCities = numberOFcities;
	lowerBound = 0;
	if (n == NULL)
	{
		path.push_back(0);
	}
	else
	{
		path.assign(n->path.begin(), n->path.end());
		path.push_back(name);
	}
	if (name) {
		lowerBound += this->costMatrix[path.at(path.size() - 2)][name];
		for (int i = 0; i < numberOFcities; i++) {
			this->costMatrix[path.at(path.size() - 2)][i] = -1;
			this->costMatrix[i][name] = -1;
		}
		this->costMatrix[name][path.at(path.size() - 2)] = -1;
		lowerBound += n->lowerBound;
	}
	minAndSubtract();
}

void Node::minAndSubtract()
{
	int min;

	//deleted min value for rows
	for (int i = 0; i < numberOfCities; i++)
	{
		min = INT_MAX;
		for (int j = 0; j < numberOfCities; j++)
		{
			if (costMatrix[i][j] < min && costMatrix[i][j] != -1)
				min = costMatrix[i][j];
		}
		for (int j = 0; j < numberOfCities; j++)
		{
			if (costMatrix[i][j] != -1)
				costMatrix[i][j] -= min;
		}
		if (min != INT_MAX) {
			lowerBound += min;
		}


	}

	for (int i = 0; i < numberOfCities; i++)
	{
		min = INT_MAX;
		for (int j = 0; j < numberOfCities; j++)
		{
			if (costMatrix[j][i] < min && costMatrix[j][i] != -1)
				min = costMatrix[j][i];
		}
		for (int j = 0; j < numberOfCities; j++)
		{
			if (costMatrix[j][i] != -1)
				costMatrix[j][i] -= min;
		}
		if (min != INT_MAX)
		{
			lowerBound += min;
		}

	}


}


std::vector <Node*> *Node::generateChildren()
{
	std::vector <Node*>* children = new std::vector<Node*>;
	int size = numberOfCities - path.size();
	bool *isParent = new bool[numberOfCities];
	for (int i = 0; i < numberOfCities; i++)
	{
		isParent[i] = false;
	}
	for (auto it = path.begin(); it != path.end(); it++)
	{
		isParent[*it] = true;
	}

	for (int i = 0; i < numberOfCities; i++)
	{
		if (!isParent[i])
		{
			Node *n = new Node(this->costMatrix, this->numberOfCities, this, i);
			children->push_back(n);
		}
	}
	delete[] isParent;
	return children;
}

bool Node::endPath()
{
	if (path.size() == numberOfCities)
		return true;
	else return false;
}