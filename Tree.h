#pragma once
#include <functional>
#include <vector>
#include <iostream>
#include "Diff_evolution/DiffEvolution.h"



using namespace std;

class Tree
{
private:
	
	int numberFunc;//����� ������� ������� ������������ � ����
	int numVertices = 0;//���������� ������
	int numNodes;//���������� ����� ����
	int layerLevel;//�� ����� ������ ������������ ������ ��������� ����
	int numCluster = 2;//���������� ���������
	int size;//���������� ������

	double coef = NULL;//����������� � ��������
	int numInput = NULL;//����� �����
	int ammInputs;//���������� ������

	double fitness=-9999999;//�� ��� �������

	bool unarFuncUs = false;//������������ �� ������� ������� true/false
	bool lastVertice = false;//���������� �� ���������
	
	int* label = nullptr;//����������� ������
	Tree* left = nullptr;
	Tree* right = nullptr;


	vector<string> strUnarFunc = { "x","sin","cos","ln","exp" };//���������� ����� �������
	vector<string> strBinaryFunc = { "+","-","*","/" };//���������� ����� �������
	vector<function <double(double)>> unarFunc = {
		[](double x) {return sin(x); },
		[](double x) {return cos(x); },
		[](double x) {if (x == 0) return 100000.0; return log(abs(x)); },
		[](double x) {if (x >= 10) return exp(10); if (x <= -10) return(0.0); return exp(x); }

	};//������� �� ������� �������
	vector<function <double(double, double)>> binaryFunc = {
		[](double x,double y) {return x + y; },
		[](double x,double y) {return x - y; },
		[](double x,double y) {return x * y; },
		[](double x,double y) {if (y == 0) return 1.0; return x / y; }
	};//������� �� �������� �������


	double distanceAverageIn(double** data, int* label, int str, int cluster);
	double distanceAverageOut(double** data, int* label, int str);
	void clustering(double** data, int str);

public:
	Tree() {}
	Tree(const Tree &copy) :numberFunc(copy.numberFunc), lastVertice(copy.lastVertice),
		unarFuncUs(copy.unarFuncUs), coef(copy.coef),numVertices(copy.numVertices),numNodes(copy.numNodes),fitness(copy.fitness),
		layerLevel(copy.layerLevel), numInput(copy.numInput), ammInputs(copy.ammInputs), numCluster(copy.numCluster)
	{

		if (copy.label != nullptr) {
			if (label != nullptr) {
				delete[] label;
				label = nullptr;
			}
			label = new int[copy.size];
			size = copy.size;
			for (int i = 0; i < size; i++) {
				label[i] = copy.label[i];
			}
		}

		/*this->operator=(copy);
		cout << 1;*/
		//��������� ������ ����� �� ���� ���� ��������������� ���������
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
	void calcFitness(double** x, int size,double K1);
	double getFitness() {
		return fitness;
	}
	Tree(int d,int numInputs);
	string getFunc();
	int* getLabel() {
		return label;
	}

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
		if (label != nullptr) {
			delete[] label;
			label = nullptr;
		}
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

	void trainWithDE(double** x, int size, double K1);




	void randFunc() {//������������ ��� ��������� �������
		if (lastVertice) {
			int r = rand() % (ammInputs + 1);//��������� � ����
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

		if (copy.label != nullptr) {
			if (label != nullptr) {
				delete[] label;
				label = nullptr;
			}
			label = new int[copy.size];
			size = copy.size;
			for (int i = 0; i < size; i++) {
				label[i] = copy.label[i];
			}
			
		}



		numCluster = copy.numCluster;

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
		//��������� ������ ����� �� ���� ���� ��������������� ���������
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

