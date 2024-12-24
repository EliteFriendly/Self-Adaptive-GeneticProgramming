#include <iostream>
#include <vector>
#include "MutationGP.h"
#include "Tree.h"
#include "CrossoverGP.h"
#include <fstream>
#include "AdaptiveGeneticProgramming.h"
#include <time.h>
using namespace std;
int dimension = 1;
const double PI = 3.1415926535;

double func0(double x) {
	return x;
}
double func1(double x) {
	return 3*sin(x) + 5*cos(x);
}


double func2(double x) {
	return pow(10*x,0.5);
}
double func3(double* x) {
	return pow(x[0], 1.5) + 10 * sin(x[0]);
}


double funcTest(double* x) {
	return x[0] + x[1];
}

double funcRastrigin(double *x) {
	double y = 0;
	for (int i = 0; i < dimension; i++) {
		y += (pow(x[i], 2) - cos(2 * PI * x[i]) + 10);
	}
	return y;
}

double funcRosenbrock(double* x) {
	double y = 0;
	for (int i = 0; i < dimension - 1; i++) {
		y += (100 * pow(pow(x[i], 2) - x[i + 1], 2) + pow(x[i] - 1, 2));
	}
	return y;
}





/*
* ��������� 
* random_device rd;

		mt19937_64 gen(rd());
		��������� ��� �����������

���� �������� ���������� ������ ��� 1 ������


!!!�� ���� ��� ������ ���� �������������� ��������� �������� �� ����������

*�� �� ���� �������� ����� ��������� ��������:


*��� ����� ��������:
������� ����� ������������ ���������� �� ������� ��������




*����������� ������:

� �������� 5 ��� ����������� ������ ���������

�� ��������� ������ ����������� ���������

*/

void yi(int* in) {
	in[0];
}


double addNoise(double x, int power) {
	if (rand() % 2) {
		return (x + double(rand() % power + rand() % 1) / 100.0 * x);
	}
	else {
		return (x - double(rand() % power + rand() % 1) / 100.0 * x);
	}
}

void qsortRecursive(int* mas, int size) {
	// ��������� � ������ � � ����� �������
	int i = 0;
	int j = size - 1;
	// ����������� ������� �������
	int mid = mas[size / 2];
	// ����� ������
	do {
		// ��������� ��������, ���� ��, ������� ����� ���������� � ������ �����
		// � ����� ����� ������� ���������� (��������� �� �����) ��������, ������� ������ ������������
		while (mas[i] < mid) {
			i++;
		}
		// � ������ ����� ���������� ��������, ������� ������ ������������
		while (mas[j] > mid) {
			j--;
		}
		// ������ �������� �������
		if (i <= j) {
			int tmp = mas[i];
			mas[i] = mas[j];
			mas[j] = tmp;
			i++;
			j--;
		}
	} while (i <= j);
	// ����������� ������, ���� ��������, ��� �����������
	if (j > 0) {
		qsortRecursive(mas, j + 1);
	}
	if (i < size) {
		qsortRecursive(&mas[i], size - i);
	}
}



void doResearch(int number, int setRand) {

	int points = 100;
	double** x = new double* [points];
	double* y = new double[points];
	srand(setRand);
	mt19937 gen(rand());
	uniform_real_distribution<> dist(0, 10);
	ofstream out("Points/Out_"+to_string(number) + ".txt");

	for (int i = 0; i < points; i++) {
		x[i] = new double[dimension];
		for (int j = 0; j < dimension; j++)
			x[i][j] = dist(gen);
	}


	for (int i = 0; i < points; i++) {
		y[i] = addNoise(func3(x[i]), 10);
		for (int j = 0; j < dimension; j++)
			out << x[i][j] << '\t';
		out << y[i] << endl;
	}


	//srand(10);
	//������������ ������� ��� ��������

	int i1 = 0;//��������
	int i2 = 0;

	double** XTrain = new double* [(int(0.8 * points))];
	double** XTest = new double* [(int(0.2 * points))];

	double* YTrain = new double[(int(0.8 * points))];
	double* YTest = new double[(int(0.2 * points))];

	for (int i = 0; i < points; i++) {
		if (i % 5 != 0) {
			XTrain[i1] = new double[dimension];
			for (int j = 0; j < dimension; j++)
				XTrain[i1][j] = x[i][j];

			YTrain[i1] = y[i];
			i1++;
		}
		else {
			XTest[i2] = new double[dimension];
			for (int j = 0; j < dimension; j++)
				XTest[i2][j] = x[i][j];

			YTest[i2] = y[i];
			i2++;
		}
	}


	ofstream file1("Points/Train_" + to_string(number) + ".txt");
	ofstream file2("Points/Test_" + to_string(number) + ".txt");


	for (int i = 0; i < (int(0.8 * points)); i++) {
		for (int j = 0; j < dimension; j++)
			file1 << XTrain[i][j] << '\t';
		file1 << YTrain[i] << endl;
	}
	for (int i = 0; i < (int(0.2 * points)); i++) {
		for (int j = 0; j < dimension; j++)
			file2 << XTest[i][j] << '\t';
		file2 << YTest[i] << endl;
	}

	AdaptiveGeneticProgramming proba(0.7, 3);
	proba.numFile(number);
	proba.startTrain(XTrain, dimension, YTrain, int(0.8*points), 50, 50);


	ofstream res("Results/Input_" + to_string(number) + ".txt");

	res << proba.getBest().getFunc()<<endl;
	res << proba.getError(XTest, YTest, int(0.2 * points));



	out.close();
	res.close();
	file1.close();
	file2.close();

	for (int i = 0; i < points; i++)
		delete[] x[i];
	for (int i = 0; i < int(0.2*points); i++)
		delete[] XTest[i];
	for (int i = 0; i < int(0.8 * points); i++)
		delete[] XTrain[i];

	delete[] x;
	delete[] XTest;
	delete[] XTrain;

	delete[] y;
	delete[] YTest;
	delete[] YTrain;
}




void main() {
	
	int str = 5;
	int col = 1;

	double** X = new double* [str];

	for (int i = 0; i < str; i++) {
		X[i] = new double[col];
		X[i][0] = i;
	}

	//cout<<distanceAvereage(X, str, col);






	//srand(1);
	////cout << "����� ������������ = " << i<<endl;
	//AdaptiveGeneticProgramming proba(1.2, 3);
	//proba.startTrain(x, 1, y, ammount, 10, 10);
	//cout << proba.getBest().getFunc();

}