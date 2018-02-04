#include "TSP.h"
#include "Tabu.h"
#include "GeneticAlgorithm.h"
#include <iostream>
#include <ctime>
#include <conio.h>
#include <Windows.h>


void menu();
LARGE_INTEGER startTimer()
{
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return start;
}
LARGE_INTEGER endTimer()
{
	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return stop;
}

int main()
{
	srand(time(NULL));
	menu();
	return 0;
}

void menu() {
	using namespace std;

	TSP *T;
	T = NULL;
	Tabu *P;
	P = NULL;  
	GeneticAlgorithm *G;
	G = NULL;
	int choice;
	int iterations = 100; // ILOŒÆ ITERACJI
	do
	{
		system("cls");
		cout << "-------------MENU---------------" << endl;
		cout << "1. Wczytaj dane i wyswietl." << endl;
		cout << "2. Algorytm" << endl;
		cout << "3. Losuj dane i wyswietl" << endl;
		cout << "4. Czas" << endl;
		cout << "0. Koniec" << endl;
		
		cin >> choice;
		switch (choice)
		{
		case 1:
		{
			system("cls");
			cout << "Podaj nazwe pliku" << endl;
			string name;
			cin >> name;
			T = new TSP(name);
			P = new Tabu(name, iterations);
			G = new GeneticAlgorithm(name);
			T->print();
			cin.ignore();
			cin.get();
			break;
		}
		case 2:
		{
			system("cls");
			cout << "1. Branch & Bound" << endl;
			cout << "2. Tabu Search" << endl;
			cout << "3. Algorytm genetyczny" << endl;
			cin >> choice;
			switch(choice) {
			case 1: 
			{
				T->BB();
				T->printResult();
				cin.ignore();
				cin.get();
				break;
			}
			case 2:
			{
				P->invoke();
				cin.ignore();
				cin.get();
				break;
			}
			case 3:
			{
				G->algorithm();
				cin.ignore();
				cin.get();
			}
			default:
				break;
			}
			break;

		}
		case 3:
		{
			system("cls");
			cout << " Podaj liczbe miast" << endl;

			int cities;
			cin >> cities;

			T = new TSP(cities);
			T->print();
			break;
		}
		case 4:
		{
			system("cls");
			cout << "1. Branch & Bound" << endl;
			cout << "2. Tabu Search" << endl;
			cin >> choice;
			switch (choice) {
			case 1:
			{
				LARGE_INTEGER performanceCountStart, performanceCountEnd;
				unsigned _int64 freq;
				QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
				system("cls");


				performanceCountStart = startTimer();
				T->BB();
				T->printResult();
				performanceCountEnd = endTimer();
				double tm = (performanceCountEnd.QuadPart - performanceCountStart.QuadPart) / (double)freq * 1000;
				cout << " Czas = " << tm << "[ms]" << endl;
				cin.ignore();
				cin.get();
				break;
			}
			case 2:
			{
				LARGE_INTEGER performanceCountStart, performanceCountEnd;
				unsigned _int64 freq;
				QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
				system("cls");

					performanceCountStart = startTimer();
					P->invoke();
					performanceCountEnd = endTimer();
					double tm = (performanceCountEnd.QuadPart - performanceCountStart.QuadPart) / (double)freq * 1000;
					cout << " Czas = " << tm << "[ms]" << endl;
					cin.ignore();
					cin.get();
					break;
			}
			case 3:
			{
				LARGE_INTEGER performanceCountStart, performanceCountEnd;
				unsigned _int64 freq;
				QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
				system("cls");

				performanceCountStart = startTimer();
				G->algorithm();
				performanceCountEnd = endTimer();
				double tm = (performanceCountEnd.QuadPart - performanceCountStart.QuadPart) / (double)freq * 1000;
				cout << " Czas = " << tm << "[ms]" << endl;
				cin.ignore();
				cin.get();
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	} while (choice);
}

