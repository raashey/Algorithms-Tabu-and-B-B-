#include "TSP.h"
#include "Node.h"
#include <queue>
using namespace std;

TSP::TSP()	//konstruktor bezparametrowy
{
	cin >> numberOFcities;							//tworze samodzielnie macierz
	costMatrix = new int*[numberOFcities];			//podajac ilosc miast oraz
	for (int i = 0; i < numberOFcities; i++)		//konkretne wartosci w macierzy kosztow
	{
		costMatrix[i] = new int[numberOFcities];
		for (int j = 0; j < numberOFcities; j++)
			cin >> costMatrix[i][j];
	}
}

TSP::TSP(int size)					//konstruktor parametrowy
{
	randomMatrix(size);				//wywo³ujê funkcjê tworz¹c¹ losow¹ macierz o zadanej iloœæ miast
}

TSP::TSP(string name) {				//konstruktor parametrowy
	readMatrixFromFile(name);		//wywo³ujê funkcjê wczytuj¹c¹ macierz kosztow z pliku o danej nazwie
}


TSP::~TSP()							//destruktor
{
	for (int i = 0; i < numberOFcities; i++)		//usuwam macierz kosztow
		delete[] costMatrix[i];
	delete[] costMatrix;
}




void TSP::print()				//funkcja drukujaca macierz
{
	cout << "  ";
	for (int i = 0; i < numberOFcities; i++) {
		if (i < 10)
			cout << setw(4) << "  " << i;
		else
			cout << setw(3) << "  " << i;
	}
	cout << endl;
	for (int i = 0; i < numberOFcities; i++)
	{
		if (i > 9)
			cout << i << " ";
		else
			cout << i << "  ";
		for (int j = 0; j < numberOFcities; j++)
			cout << setw(4) << costMatrix[i][j] << " ";
		cout << endl;
	}
}
class cmpLowerBound
{
public:
	bool operator() (const Node *node1, const Node *node2)			//stworzenie funktora sortuj¹cego tak aby porownac dolne ograniczenia w wierzcho³kach 
	{
		if (node1->lowerBound > node2->lowerBound) return true;
		else return false;
	}
};
void TSP::randomMatrix(int size) {					//funkcja tworzaca losowa macierz o zadanej ilosci miast
	int choice;
	int weight;

	system("cls");										//wybor rodzaju macierzy, poniewaz ró¿ni¹ siê budow¹
	cout << "1. Symetryczna" << endl;
	cout << "2. Asymetryczna" << endl;
	cout << "Wybor: ";
	cin >> choice;
	system("cls");
	cout << "Podaj maksymalna calkowita wage sciezki: ";		//sprecyzowanie maksymalnej wagi sciezki
	cin >> weight;

	switch (choice) {
	case 1: {
		numberOFcities = size;
		costMatrix = new int*[numberOFcities];
		for (auto i = 0; i < numberOFcities; ++i)
		{
			costMatrix[i] = new int[numberOFcities];
		}
		for (auto i = 0; i < numberOFcities; i++)
		{
			for (auto j = 0; j <= i; j++)
			{
				if (i == j)
				{
					costMatrix[i][j] = -1;
				}
				else
				{
					costMatrix[i][j] = rand() % weight;
					costMatrix[j][i] = costMatrix[i][j];
				}
			}
		}
		break;
	}
	case 2: {
		numberOFcities = size;
		costMatrix = new int*[numberOFcities];
		for (int i = 0; i < numberOFcities; i++)
		{
			costMatrix[i] = new int[numberOFcities];
			for (int j = 0; j < numberOFcities; j++)
			{
				if (i == j)
				{
					costMatrix[i][j] = -1;
				}
				else
				{
					costMatrix[i][j] = rand() % weight;
				}
			}
		}
		break;
	}
	}

}
void TSP::BB()
{
	priority_queue <Node*, vector<Node*>, cmpLowerBound> kolejka;				//kolejka priorytetowa, sortowana wed³ug dolnego ograniczenia
	Node *n = new Node(costMatrix, numberOFcities, NULL, 0);					//dodanie wierzcho³ka
	kolejka.push(n);															//wstawienie do kolejki

	int minCost = INT_MAX;
	endNode = NULL;
	while (!kolejka.empty())
	{
		Node * n = kolejka.top();												//przypisanie do wskaŸnika n pierwszego elementu kolejki
		kolejka.pop();															//usuniêcie pierwszego elementu z kolejki

		if (n->endPath())														//sprawdzenie czy to ostatni wierzcho³ek w aktualnej kolejce
		{
			if (minCost > n->lowerBound)
			{
				endNode = n;													//oznaczenie ostatniego wierzcho³ka
				minCost = n->lowerBound;										//ustawienie minimalnego kosztu na tym poziomie jako dolnego ograniczenia na tym wierzcho³ku
			}
			continue;
		}
		if (n->lowerBound > minCost)											//sprawdzenie czy dolne ograniczenie jest wiêksze ni¿ minimalny koszt
		{
			continue;
		}
		vector<Node*>* children = n->generateChildren();						//stworzenie wektora wierzcho³ków do których komiwoja¿er mo¿e iœæ z danego wierzcho³ka
		for (auto it = children->begin(); it != children->end(); it++)
		{
			kolejka.push(*it);													//dodanie do kolejki wskaŸników na te wierzcho³ki
		}
		children->clear();														//wyczyszczenie wektora
		children->shrink_to_fit();												//zmniejszenie pojemnoœci wektora do jego rozmiaru

	}
}

void TSP::printResult()
{
	cout << "Rozwiazanie: \n" << endNode->lowerBound << endl;						//wydrukowanie wynikow w buforze
	cout << "Path: ";
	for (auto it = endNode->path.begin(); it != endNode->path.end(); it++)				//przejscie po kolei sciezki i wypisanie jej
	{
		cout << *it << "-> ";
	}
	cout << endNode->path.at(0) << endl;
}
void TSP::readMatrixFromFile(string name) {												//parser do wczytywania plikow .tsp, .atsp, .txt

	cout << "Reading" << endl;

	string line, lineTmp;

	size_t found1 = name.find(".atsp");
	size_t found2 = name.find(".tsp");
	size_t found3 = name.find(".txt");
	if (found1 != string::npos){

		ifstream sourceFile(name);


		if (sourceFile.is_open())
		{
			while (getline(sourceFile, line) && (line != "EDGE_WEIGHT_SECTION"))
			{
				if (line.find("DIMENSION:") != string::npos)
				{
					stringstream s = stringstream(line);
					s >> lineTmp;
					s >> numberOFcities;
				}
			}

			costMatrix = new int*[numberOFcities];
			for (auto i = 0; i < numberOFcities; ++i)
			{
				costMatrix[i] = new int[numberOFcities];
			}
			for (auto i = 0; i < numberOFcities; ++i)
			{
				for (auto j = 0; j < numberOFcities; ++j)
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
					s >> numberOFcities;
				}
			}
			costMatrix = new int*[numberOFcities];
			for (auto i = 0; i < numberOFcities; ++i)
			{
				costMatrix[i] = new int[numberOFcities];
			}
			for (auto i = 0; i < numberOFcities; i++)
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
			plik >> numberOFcities;
			costMatrix = new int*[numberOFcities];
			for (int i = 0; i < numberOFcities; i++)
			{
				costMatrix[i] = new int[numberOFcities];
				for (int j = 0; j < numberOFcities; j++)
					plik >> costMatrix[i][j];
			}
		}
		else
			cout << "Bledna nazwa pliku!" << endl;
		plik.close();
	}
		
}

