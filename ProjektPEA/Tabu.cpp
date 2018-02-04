#include "Tabu.h"

using namespace std;

//KONSTRUKTOR DOMYŒLNY
Tabu::Tabu()
{
	cin >> problemSize;
	costMatrix = new int*[problemSize];			//podajac ilosc miast oraz
	for (int i = 0; i < problemSize; i++)		//konkretne wartosci w macierzy kosztow
	{
		costMatrix[i] = new int[problemSize];
		for (int j = 0; j < problemSize; j++)
			cin >> costMatrix[i][j];
	}
	tabuList = new TabuList(problemSize);
	currSolution = new int[problemSize + 1];
	bestSolutionEver = new int[problemSize + 1];
}

//KONSTRUKTOR PARAMETRYCZNY
Tabu::Tabu(string name, int iterations) 
{
	numberOfIterations = iterations;
	readMatrixFromFile(name);

	tabuList = new TabuList(problemSize);
	currSolution = new int[problemSize + 1];
	bestSolutionEver = new int[problemSize + 1];
}

//DESTRUKTOR
Tabu::~Tabu()
{
	for (int i = 0; i < problemSize; i++)		//usuwam macierz kosztow
		delete[] costMatrix[i];
	delete[] costMatrix;
	delete[] currSolution;
	delete[] bestSolutionEver;
}

//PARSER PLIKÓW .TSP,.ATSP,.TXT
void Tabu::readMatrixFromFile(string name) {												//parser do wczytywania plikow .tsp, .atsp, .txt

	cout << "Reading" << endl;

	string line, lineTmp;

	size_t found1 = name.find(".atsp");
	size_t found2 = name.find(".tsp");
	size_t found3 = name.find(".txt");
	if (found1 != string::npos) {

		ifstream sourceFile(name);


		if (sourceFile.is_open())
		{
			while (getline(sourceFile, line) && (line != "EDGE_WEIGHT_SECTION"))
			{
				if (line.find("DIMENSION:") != string::npos)
				{
					stringstream s = stringstream(line);
					s >> lineTmp;
					s >> problemSize;
				}
			}

			costMatrix = new int*[problemSize];
			for (auto i = 0; i < problemSize; ++i)
			{
				costMatrix[i] = new int[problemSize];
			}
			for (auto i = 0; i < problemSize; ++i)
			{
				for (auto j = 0; j < problemSize; ++j)
				{
					sourceFile >> costMatrix[i][j];
					if (costMatrix[i][j] == 9999)
						costMatrix[i][j] = -1;
				}
			}
			sourceFile.close();
		}
		else
		{
			cout << "Couldn't open file" << endl;
		}
		cout << "Finished reading" << endl;
	}

	else if (found2 != string::npos) {
		ifstream sourceFile(name);

		if (sourceFile.is_open())
		{
			while (getline(sourceFile, line) && (line != "EDGE_WEIGHT_SECTION"))
			{
				if (line.find("DIMENSION:") != string::npos)
				{
					stringstream s = stringstream(line);
					s >> lineTmp;
					s >> problemSize;
				}
			}
			costMatrix = new int*[problemSize];
			for (auto i = 0; i < problemSize; ++i)
			{
				costMatrix[i] = new int[problemSize];
			}
			for (auto i = 0; i < problemSize; i++)
			{
				for (auto j = 0; j <= i; j++)
				{
					sourceFile >> costMatrix[i][j];
					costMatrix[j][i] = costMatrix[i][j];
					if (costMatrix[i][j] == 0)
						costMatrix[i][j] = -1;
				}
			}
			sourceFile.close();
		}
		else
		{
			cout << "Couldn't open file" << endl;
		}
		cout << "Finished reading" << endl;
	}
	else if (found3 != string::npos) {

		fstream plik;
		plik.open(name, ios::in);
		if (plik.good())
		{
			plik >> problemSize;
			costMatrix = new int*[problemSize];
			for (int i = 0; i < problemSize; i++)
			{
				costMatrix[i] = new int[problemSize];
				for (int j = 0; j < problemSize; j++)
					plik >> costMatrix[i][j];
			}
		}
		else
			cout << "Bledna nazwa pliku!" << endl;
		plik.close();
	}

}

//LICZENIE DYSTANSU ŒCIE¯KI
int Tabu::calculateDistance(int* solution)
{
	int cost = 0;
	for (int i = 0; i < problemSize; i++) {
		cost += costMatrix[solution[i]][solution[i + 1]];
	}
	return cost;
}

//PRZYPISANIE ŒCIE¯KI STARTOWEJ JAKO AKTUALNIE NAJLEPSZEJ
void Tabu::setupBestSolutionEver()
{
	for (int i = 0; i < problemSize + 1; i++)
	{
		bestSolutionEver[i] = currSolution[i];
	}
	bestCostEver = calculateDistance(bestSolutionEver);
}

//STWORZENIE ŒCIE¯KI STARTOWEJ
void Tabu::setupCurrentSolution()
{
	int a, b;
	do
	{
		a = rand() % (problemSize - 1) + 1;
		b = rand() % (problemSize - 1) + 1;
	} while (a == b);
	currSolution[0] = 0;
	for (int i = 1; i < problemSize; i++)
		currSolution[i] = i;
	currSolution[problemSize] = 0;
	swap(a, b);
}       

//WYDRUK ROZWIAZANIA
void Tabu::printSolution(int* solution)
{
	for (int i = 0; i < problemSize ; i++) {
		cout << solution[i] << " -> ";
	}
	cout << solution[problemSize];
	cout << endl;
}

//WYWO£ANIE FUNKCJI TABU SEARCH
void Tabu::invoke()
{
	swapNeighbours bestSwap;
	setupCurrentSolution();
	setupBestSolutionEver();
	for (int i = 0; i < numberOfIterations; i++)
	{
		
		bestSwap = bestSwapValue();
		swap(bestSwap.i, bestSwap.j);
		if (calculateDistance(currSolution) < bestCostEver)
		{
			bestCost = calculateDistance(currSolution);
			setupBestSolutionEver();
		}
		else
		{
			diversification();
			continue;
		}
		if (bestSwap.i != 0 && bestSwap.j != 0)
		{
			tabuList->decrementTabu();
			tabuList->tabuMove(bestSwap.i, bestSwap.j);
		}
		//cout << "Iteracja nr : " << i << endl;
	}
	cout << "Search done! \nBest Solution cost found = " << bestCostEver << "\nBest Solution : ";
	printSolution(bestSolutionEver);
}

//ZAMIANA MIAST W ŒCIE¯CE
void Tabu::swap(int a, int b)
{
	int temp = currSolution[a];
	currSolution[a] = currSolution[b];
	currSolution[b] = temp;
}

//SZUKANIE NAJLEPSZEGO S¥SIEDZTWA
swapNeighbours Tabu::bestSwapValue()
{


	swapNeighbours bestSwap;
	bool checker = true;
	int bestSwapValue = 9999;
	int currCost;  
	float aspiration = 1.15f;// KRYTERIUM ASPIRACJI

	for (int j = 1; j < problemSize; j++) {
		for (int k = 2; k < problemSize; k++) {
			if (j != k) {
				swap(j, k);
				currCost = calculateDistance(currSolution);
				if (bestSwapValue > (currCost - bestCost) && tabuList->tabuList[bestSwap.i][bestSwap.j] == 0) 
				{
					bestSwapValue = currCost - bestCost;
					bestSwap.i = j;
					bestSwap.j = k;
					checker = true;
				}
				else if (bestSwapValue > (currCost - bestCost)* aspiration && tabuList->tabuList[bestSwap.i][bestSwap.j] != 0)
				{
					bestSwapValue = currCost - bestCost;
					bestSwap.i = j;
					bestSwap.j = k;
					checker = false;
				}
			}
			swap(k, j);
		}
	}
	if (checker == false)
		tabuList->tabuList[bestSwap.i][bestSwap.j] = 0;
	return bestSwap;
}

//DYWERSYFIKACJA
void Tabu::diversification()
{
	int a, b;
	do
	{
		a = rand() % (problemSize - 1) + 1;
		b = rand() % (problemSize - 1) + 1;
	} while (a == b);
	currSolution[0] = 0;
	for (int i = 1; i < problemSize; i++)
		currSolution[i] = i;
	currSolution[problemSize] = 0;
	swap(a, b);
	tabuList->resetTabuList();
}