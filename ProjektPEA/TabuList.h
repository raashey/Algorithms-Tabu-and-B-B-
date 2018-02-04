#include <iostream>

using namespace std;

#pragma once
class TabuList
{
public:

	int problemSize;
	int ** tabuList;
	int cadence;

	TabuList();
	TabuList(int x);
	~TabuList();
	void tabuMove(int x, int y);
	void decrementTabu();
	void resetTabuList();
};

