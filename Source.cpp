#include <iostream>
#include <vector>
#include "MutationGP.h"
#include "Tree.h"
#include "CrossoverGP.h"
#include <fstream>
#include "AdaptiveGeneticProgramming.h"
#include <time.h>
#include <string>
#include <fstream>
#include "sample_storage.h"
using namespace std;
int dimension = 1;
const double PI = 3.1415926535;
static const int DATA_SIZE = 1000;

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
* ЗАПОМНИТЬ 
* random_device rd;

		mt19937_64 gen(rd());
		ГЕНЕРАТОР БЕЗ ОГРАНИЧЕНИЙ

ЭТОТ АЛГОРИТМ РЕАЛИЗОВАН ТОЛЬКО ДЛЯ 1 ВЫХОДА


!!!Не знаю что делать если функциональное множества меняется на термальное

*Из за чего алгоритм может перестать работать:


*Что нужно изменить:
Сделать поиск минимального расстояния до другого кластера




*Оптимизация момент:

В СЕЛЕКЦИИ 5 РАЗ СОРТИРУЕТСЯ МАССИВ ИНДИВИДОВ

НЕ УДАЛЯЕТСЯ МАССИВ РАЗМЕЧЕННЫХ ИНДИВИДОВ

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
	// Указатели в начало и в конец массива
	int i = 0;
	int j = size - 1;
	// Центральный элемент массива
	int mid = mas[size / 2];
	// Делим массив
	do {
		// Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
		// В левой части массива пропускаем (оставляем на месте) элементы, которые меньше центрального
		while (mas[i] < mid) {
			i++;
		}
		// В правой части пропускаем элементы, которые больше центрального
		while (mas[j] > mid) {
			j--;
		}
		// Меняем элементы местами
		if (i <= j) {
			int tmp = mas[i];
			mas[i] = mas[j];
			mas[j] = tmp;
			i++;
			j--;
		}
	} while (i <= j);
	// Рекурсивные вызовы, если осталось, что сортировать
	if (j > 0) {
		qsortRecursive(mas, j + 1);
	}
	if (i < size) {
		qsortRecursive(&mas[i], size - i);
	}
}



void doResearch(int number, int filename) {

	
	int str = 200;
	int col = 2;

	ifstream file("Num_"+to_string(number)+".txt");


	double** data = new double* [str];
	double tmp;

	for (int i = 0; i < str; i++) {
		data[i] = new double[col];
		for (int j = 0; j < col; j++) {
			file >> data[i][j];
		}
		file >> tmp;
	}
	ofstream out("Points/Out_"+to_string(number) + to_string(filename) + ".txt");

	

	AdaptiveGeneticProgramming proba(1, 3);
	//proba.numFile(filename+number*10);
	proba.startTrain(data, col, str, 20, 20);


	ofstream res("Results/Input_" + to_string(number) + to_string(filename) + ".txt");

	res << proba.getBest().getFunc()<<endl;
	res << proba.getBest().getFitness() << endl;;

	for (int i = 0; i < str; i++) {
		for (int j = 0; j < col; j++) {
			out << data[i][j]<<'\t';
		}
		out << proba.getBest().getLabel()[i];
		out << endl;
	}

	out.close();
	res.close();


	for (int i = 0; i < str; i++)
		delete[] data[i];

	delete[] data;

}

void test(string path, int dim, string mark)
{
    ifstream file(path);
    if (!file.is_open())
    {
        cerr << "Error opening files" << endl;
        exit(1);
    }
    // Read data from file
    double **data = new double *[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++)
    {

        data[i] = new double[dim+1];

        for (int j = 0; j < dim+1; j++)
        {
            file >> data[i][j];
            //cout<<data[i][j]<<" ";
            if (file.peek() == ',')
                file.ignore();
            // cout << data[i][j] << " ";
        }
        //cout << endl;

        // cout << endl;
    }
    file.close();
    SampleStorage storage(DATA_SIZE, dim, data, 0.75, "reg"); // 75% for training

    int treeDepth = 4; // depth of tree

    ofstream fileOut("algorithm_results/Results/Best_" + mark + ".txt");
    if (!fileOut.is_open())
    {
        cout << "Error opening file out" << endl;
        exit(1);
    }
    ofstream filePoints("algorithm_results/Points/" + mark + ".txt");
    if (!filePoints.is_open())
    {
        cout << "Error opening file out" << endl;
        exit(1);
    }

    cout << "Iteration " << mark << endl;
    AdaptiveGeneticProgramming proba(1,treeDepth, "reg");
    proba.numFileAndTrail(mark,true);

	proba.startTrain(data, dim, DATA_SIZE, 11, 11);
	Tree best = proba.getBest();
    // fileOut << proba.getError(dataTest, size * 0.25) << endl;
    fileOut << best.getFunc() << endl;
    
    fileOut.close();
    for (int i = 0;i < storage.getTestSize();i++) {
        filePoints << storage.getTestData()[i][dim] << " " << best.getValue(storage.getTestData()[i]) << endl;
    }
    filePoints.close();
}


int main()
{
	srand(2);
	setlocale(0 , "");
	clock_t tStart = clock();

    std::vector<std::string> file_names = {
        "I_6_2b.txt",
        "I_8_14.txt", 
        "I_12_1.txt",
        "I_12_2.txt",
        "I_12_4.txt",
        "I_14_3.txt",
        "I_14_4.txt",
        "I_15_3x.txt",
        "I_15_10.txt",
        "I_18_4.txt",
        "I_24_6.txt",
        "I_32_5.txt"
    };

// Массив количества изменяемых параметров для каждой задачи
    std::vector<int> parameter_counts = {
        3,  // I.6.2b: 
        4,  // I.8.14: x1, y1, x2, y2
        2,  // I.12.1: q1, q2, r
        4,  // I.12.2: q, E, v, B
        3,  // I.12.4: mu, r
        3,  // I.14.3: m, h
        2,  // I.14.4: k, x
        4,  // I.15.3x: x1, u, t
        3,  // I.15.10: m, v
        4,  // I.18.4: m, v, r, theta
        4,  // I.24.6: n, theta2
        4   // I.32.5: q, a
    };
    string st = "test/" + file_names[0];
    //cout << st << endl;
    ifstream file(st);
    if (!file.is_open())
    {
        cerr << "Error opening files" << endl;
        exit(1);
    }
    


    // cout << endl;
    int l = 8;
    double* coef = new double[l];
    for (int i = 0; i < l; i++)
    {
        coef[i] = 1;
    }
    /*Tree proba(3 , 2 , 1 , "reg");
    proba.doNeuronNetwork();
    proba.changeCoef(coef);
    cout << proba.getValue(coef)[0] << endl;
    cout << proba.getValue(coef)[0] << endl;
    cout << proba.getValue(coef)[0] << endl;
    cout << proba.getValue(coef)[0] << endl;
    cout << proba.getValue(coef)[0] << endl;
    cout << proba.getFunc() << endl;
    cout << proba.getMatrix() << endl;*/


    try {
    for (int i = 0;i < file_names.size();i++) {
        for (int r = 0; r < 10; r++)
        {
            test("test/"+file_names[i], parameter_counts[i],to_string(i)+to_string(r));
        }
    }
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }




    cout << "Good";

    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}