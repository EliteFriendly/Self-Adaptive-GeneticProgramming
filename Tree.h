#pragma once
#include <functional>
#include <vector>
#include <iostream>
#include "Diff_evolution/DiffEvolution.h"



using namespace std;

class Tree
{
private:
	
	int numberFunc;//Номер функции который используется в узле
	int numVertices = 0;//Количество вершин
	int numNodes;//Количество узлов ниже
	int layerLevel;//На каком уровне относительно начала находится узел


	double coef = NULL;//Коэффициент в вершинах
	int numInput = NULL;//Номер входа
	int ammInputs;//Количество входов

	double fitness=-9999999;//Ну тут понятно

	bool unarFuncUs = false;//Используется ли унарная функция true/false
	bool lastVertice = false;//Термальное ли множество

	Tree* left = nullptr;
	Tree* right = nullptr;


	vector<string> strUnarFunc = { "x","sin","cos","ln","exp" };//Символьный вывод функции
	vector<string> strBinaryFunc = { "+","-","*","/" };//Символьный вывод функции
	vector<function <double(double)>> unarFunc = {
		[](double x) {return sin(x); },
		[](double x) {return cos(x); },
		[](double x) {if (x == 0) return 100000.0; return log(abs(x)); },
		[](double x) {if (x >= 10) return exp(10); if (x <= -10) return(exp(-10)); return exp(x); }

	};//Выборка из унарных функций
	vector<function <double(double, double)>> binaryFunc = {
		[](double x,double y) {return x + y; },
		[](double x,double y) {return x - y; },
		[](double x,double y) {return x * y; },
		[](double x,double y) {if (y == 0) return 1.0; return x / y; }
	};//Выборка из бинарных функций


	

public:
	Tree() {}
	Tree(const Tree &copy) :numberFunc(copy.numberFunc), lastVertice(copy.lastVertice),
		unarFuncUs(copy.unarFuncUs), coef(copy.coef),numVertices(copy.numVertices),numNodes(copy.numNodes),fitness(copy.fitness),
		layerLevel(copy.layerLevel), numInput(copy.numInput), ammInputs(copy.ammInputs)
	{
		/*this->operator=(copy);
		cout << 1;*/
		//Выделение памяти чтобы не было кучи взаимосвязанных индивидлв
		if (copy.left != nullptr) {
			if (left != nullptr) {
				delete left;
			}
			left = new Tree;
			*left = Tree(*(copy.left));
		}
		else {
			if (left != nullptr) {
				delete left;
				left = nullptr;
			}
		}
		if (copy.right != nullptr) {
			if (right != nullptr) {
				delete right;
			}
			right = new Tree;
			*right = Tree(*(copy.right));
		}
		else {
			if (right != nullptr) {
				delete right;
				right = nullptr;
			}
		}
		
	}

	void calcFitness(double** x, double* y, int size,double K1);
	double getFitness() {
		return fitness;
	}
	Tree(int d,int numInputs);
	string getFunc();

	bool getUnar() {
		return unarFuncUs;
	}
	bool getLastVertice() {
		return lastVertice;
	}

	void countNodes(int&);
	void recountLayers(int);

	void changeCoef(double *,int&);
	double getNumVertices();
	int getNumFunc() {
		return numberFunc;
	}
	double getValue(double *x);
	int getNumNodes() {
		
		return numNodes;
	}
	int getAmmInputs() {
		return ammInputs;
	}
	~Tree() {
		if (left != nullptr) {
			delete left;
			left = nullptr;
		}
		if (right != nullptr) {
			delete right;
			right = nullptr;
		}
	}
	void replaceNode(int, Tree&);
	void changeNode(int, Tree&);

	void trainWithDE(double** x, double* y, int size, double K1);

	void randFunc() {//Используется для оператора мутации
		if (lastVertice) {
			int r = rand() % (ammInputs + 1);//Считается с коэф
			if (r == 0) {
				numVertices = 1;
			}
			else {
				numVertices = 0;
				numInput = rand() % ammInputs;
			}
		}
		else {
			if (unarFuncUs) {
				numberFunc = rand() % unarFunc.size();
			}
			else {
				numberFunc = rand() % binaryFunc.size();
			}
		}
	}

	Tree* getLeft() {
		if (left == nullptr) {
			return nullptr;
		}
		return left;
	}
	Tree* getRight() {
		if (right == nullptr) {
			return nullptr;
		}
		return right;
	}



	Tree operator =(const Tree& copy) {


		numberFunc = copy.numberFunc;
		layerLevel = copy.layerLevel;
		lastVertice = copy.lastVertice; 
		unarFuncUs = copy.unarFuncUs;
		coef = copy.coef;
		numInput = copy.numInput;
		numVertices = copy.numVertices;
		numNodes = copy.numNodes;
		fitness = copy.fitness;
		ammInputs = copy.ammInputs;
		//Выделение памяти чтобы не было кучи взаимосвязанных индивидлв
		if (copy.left != nullptr) {
			if (left != nullptr) {
				delete left;
			}
			left = new Tree;
			//left->operator=(*copy.left);
			*left = Tree(*(copy.left));
		}
		else {
			if (left != nullptr) {
				delete left;
				left = nullptr;
			}
		}
		if (copy.right != nullptr) {
			if (right != nullptr) {
				delete right;
			}
			right = new Tree;
			//right->operator=(*copy.right);
			*right = Tree(*(copy.right));
		}
		else {
			if (right != nullptr) {
				delete right;
				right = nullptr;
			}
		}
		return *this;
	}
};

