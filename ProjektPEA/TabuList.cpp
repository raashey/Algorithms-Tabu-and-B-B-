#include "TabuList.h"


//KONSTRUKTOR DOMYŒLNY
TabuList::TabuList()
{
	cin >> problemSize;
	tabuList = new int*[problemSize];
	for (int i = 0; i < problemSize; i++)
	{
		tabuList[i] = new int[problemSize];
		for (int j = 0; j < problemSize; j++)
		{
			tabuList[i][j] = 0;
		}
	}
	cadence = 12;
}

//KONSTRUKTOR PARAMETRYCZNY
TabuList::TabuList(int problemSize)
{
	tabuList = new int*[problemSize];
	for (int i = 0; i < problemSize; i++)
	{
		tabuList[i] = new int[problemSize];
		for (int j = 0; j < problemSize; j++)
		{
			tabuList[i][j] = 0;
		}
	}
	cadence = 12;
}

//DESTRUKTOR
TabuList::~TabuList()
{
	for (int i = 0; i < problemSize; i++)
	{
		delete[] tabuList[i];
	}
	delete[] tabuList;
}

//UMIESZCZENIE NA LIŒCIE TABU
void TabuList::tabuMove(int city1, int city2) 
{ 
	tabuList[city1][city2] += cadence;
	tabuList[city2][city1] += cadence;
	if (cadence != 0)
		cadence -= 2;
	else
		cadence = 12;

}

//DEKREMENTACJA LISTY
void TabuList::decrementTabu() {

	for (int i = 0; i < problemSize; i++) {
		for (int j = 0; j < problemSize; j++) {
			if (tabuList[i][j] > 0)
				tabuList[i][j] -= 1;
		}
	}
}

//WYCZYSZCZENIE LISTY
void TabuList::resetTabuList()
{
	for (int i = 0; i < problemSize; i++) {
		for (int j = 0; j < problemSize; j++)
			tabuList[i][j] = 0;
	}
}
