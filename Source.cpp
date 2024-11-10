#include <iostream>
#include <vector>
#include "MutationGP.h"
#include "Tree.h"
#include "CrossoverGP.h"
#include <fstream>
#include "AdaptiveGeneticProgramming.h"
#include <time.h>
using namespace std;

double func0(double x) {
	return x;
}
double func1(double x) {
	return 3*sin(x) + 5*cos(x);
}


double func2(double x) {
	return pow(10*x,0.5);
}
double func3(double x) {
	return pow(x, 1.5) + 10 * sin(x);
}

/*
* ��������� 
* random_device rd;

		mt19937_64 gen(rd());
		��������� ��� �����������

���� �������� ���������� ������ ��� 1 ������


!!!�� ���� ��� ������ ���� �������������� ��������� �������� �� ����������

*�� �� ���� �������� ����� ��������� ��������:

�������� ChosenNode � rand()%r+1 �� �� ��� �����(?)

��������� findReach � ��������


*��� ����� ��������:
�������� ������������ �� ��� ����, ����� �� ���� ������������ ������	



*����������� ������:

� �������� 5 ��� ����������� ������ ���������

*/

void yi(int* in) {
	in[0];
}


void main() {
	setlocale(0, "");
	//srand(10);
	srand(72);
	//������������ ������� ��� ��������
	int ammount = 100;
	double left = 0.2;
	double right = 10;
	double h = (right - left) / ammount;
	double** x = new double*[ammount];
	double* y = new double[ammount];


	for (int i = 0; i < ammount; i++) {
		x[i] = new double[1];
		x[i][0] = left + h * i;
		y[i] = func3(x[i][0]);
	}
	for (int i = 0; i < 1000; i++) {
		clock_t start = clock();
		srand(i);
		cout << "����� ������������ = " << i<<endl;

		thread thr([&]() {
			AdaptiveGeneticProgramming proba(1.2, 3);
			proba.startTrain(x, 1, y, ammount, 20, 100);

			});

		AdaptiveGeneticProgramming proba(1.2, 3);
		proba.startTrain(x, 1, y, ammount, 20, 100);
		thr.join();
		clock_t end = clock();
		printf("Work time: %f seconds\n", double(end - start) / CLOCKS_PER_SEC);
		exit(0);
	}

	//srand(1);
	////cout << "����� ������������ = " << i<<endl;
	//AdaptiveGeneticProgramming proba(1.2, 3);
	//proba.startTrain(x, 1, y, ammount, 10, 10);
	//cout << proba.getBest().getFunc();

}