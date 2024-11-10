#include "Tree.h"
#include <sstream>


void Tree::countNodes(int& ammount)
{
	
	if (left != nullptr) {
		left->countNodes(ammount);
	}
	if (right != nullptr) {
		right->countNodes(ammount);
	}
	numNodes = ammount;
	ammount++;
}

void Tree::recountLayers(int level)
{
	layerLevel = level;
	if (left != nullptr) {
		left->recountLayers(level+1);
	}
	if (right != nullptr) {
		right->recountLayers(level+1);
	}
}

void Tree::calcFitness(double** x, double* y, int size,double K1)
{
	double error = 0;

	for (int i = 0; i < size; i++) {
		error += pow(y[i] - getValue(x[i]), 2);
	}
	fitness = (1 / (1 + pow(error, 0.5))) * (20 - K1 * numNodes);
	if (fitness == NULL) {
		cout << "Фитнес равен NAN";
		exit(0);
	}
}

Tree::Tree(int d, int numInputs)
{
	ammInputs = numInputs;
	//Случай если дошли до самого конца
	if (d == 0) {
		lastVertice = true;
		if (rand() % (numInputs+1)) {
			numInput = rand() % numInputs;
			numVertices = 0;
		}
		else {
			numVertices = 1;
			coef = 1;
		}
		return;
	}
	
	int r = rand() % 2;
	
	
	if (r) {
		//В случае если унарная
		unarFuncUs = true;
		
		numberFunc = rand() % (unarFunc.size());
		//Tree a(d - 1);
		right = new Tree(d-1,numInputs);
	}
	else {
		//В случае если бинарная
		unarFuncUs = false;
		numberFunc = rand() % (binaryFunc.size());
		//Tree l(d - 1);
		//Tree r(d - 1);
		left = new Tree(d - 1,numInputs);
		right = new Tree(d - 1,numInputs);
	}
}


string Tree::getFunc()
{
	stringstream ss;
	if (lastVertice) {
		if (numVertices == 1) {
			ss << coef;
		}
		else {
			ss <<'X' << numInput;
		}

	}
	else {
		if (left != nullptr) {
			ss << '(';
			ss << left->getFunc();
		}
		if (unarFuncUs) {
			if (numberFunc == 0 and !lastVertice) {
				ss << '(';
			}
			else {
				ss << strUnarFunc[numberFunc];
				if (lastVertice == false) {
					ss << '(';
				}
			}
		}
		else {
			if (!lastVertice) {
				ss << strBinaryFunc[numberFunc];
			}

		}
		if (right != nullptr) {
			ss << right->getFunc();
			ss << ')';
		}
	}
	

	return ss.str();
}

void Tree::changeCoef(double *in,int &z)
{
	//Заполнение будет происходить слева направо
	if (left != nullptr) {//Идем сначала по левой стороне до конца
		left->changeCoef(in, z);
	}
	if (right != nullptr) {//Если нет ничего слева
		right->changeCoef(in, z);
	}
	if (lastVertice and numVertices == 1) {
		coef = in[z];//Замена коэффициентов в случае если все ок
		z++;//Работа с памятью!!!
	}
}

double Tree::getNumVertices()
{

	if (lastVertice) {
		return numVertices;
	}

	if (left != nullptr and right != nullptr) {
		numVertices = left->getNumVertices() + right->getNumVertices();
	}
	else {
		if (left != nullptr) {
			numVertices = left->getNumVertices();
		}
		if (right != nullptr) {
			numVertices = right->getNumVertices();
		}
	}

	return numVertices;
}

double Tree::getValue(double* x)
{
	//if (right != nullptr and left ==nullptr) {//Если справа что то есть то это точно унарная функци

	//	return unarFunc[numberFunc](right->getValue(x));
	//	
	//}
	if (lastVertice) {//Если дошли до вершины
		if (numVertices==1) {
			return coef;
		}
		if (numVertices==0) {
			return x[numInput];
		}
		else {
			cout << "Непредвиденность в getValue";
			exit(0);
		}
	}

	if (unarFuncUs) {
		return unarFunc[numberFunc](right->getValue(x));
	}
	else {//Если попались в бинарную функцию
		return binaryFunc[numberFunc](left->getValue(x),right->getValue(x));
	}
	
}

void Tree::replaceNode(int search, Tree& newNode)//Замена выбранного узла
{
	if (numNodes == search) {//Если мы дошли до узла под каким то номером
		*this = newNode;
		return;
	}
	//Поиск по другим узлам если не нашли подходящего номера
	if (left != nullptr and search <= left->getNumNodes()) {
		left->replaceNode(search, newNode);
	}
	if (right != nullptr and search <= right->getNumNodes()) {
		right->replaceNode(search, newNode);
	}
}

void Tree::changeNode(int search, Tree& newNode)//Отличие от replace в том, что не меняются остальные узлы
{
	if (numNodes == search) {//Если мы дошли до узла под каким то номером
		if (newNode.getLastVertice()) {
			//*this = newNode;
			if (newNode.getLastVertice() == lastVertice) {
				*this = newNode;
				return;
			}
			return;
		}
		if (lastVertice) {
			int tnumnodes = numNodes;
			*this = newNode;
			numNodes = tnumnodes;
		}
		if (newNode.getUnar() == unarFuncUs) {
			numberFunc = newNode.getNumFunc();
		}
		else {
			if (newNode.getUnar()) {
				unarFuncUs = true;
				numberFunc = newNode.getNumFunc();
				left->~Tree();
				left = nullptr;
			}
			else {
				unarFuncUs = false;
				numberFunc = newNode.getNumFunc();
				if (left != nullptr) {
					delete left;
				}
				left = new Tree;
				//left->operator=(*copy.left);
				*left = Tree(*(newNode.left));
				left->numNodes = -1;//Сделано для того, чтобы не было изменений в этой ветви
			}
		}
		return;
	}

	if (left != nullptr and search <= left->getNumNodes()) {
		left->changeNode(search, newNode);
	}
	if (right != nullptr and search <= right->getNumNodes()) {
		right->changeNode(search, newNode);
	}
}

void Tree::trainWithDE(double** x, double* y, int size, double K1)
{


	double **xDE = new double* [size];
	double* yDE = new double[size];
	for (int i = 0; i < size; i++) {
		xDE[i] = new double[ammInputs];
		for (int j = 0; j < ammInputs; j++) {
			xDE[i][j] = x[i][j];
		}
		yDE[i] = y[i];
	}

	int numVertices = getNumVertices();
	if (numVertices == 0) {
		calcFitness(xDE, yDE, size, K1);
		return;
	}
	function <double(double*)> func = [&](double* input) {
		int i = 0;
		changeCoef(input, i);
		calcFitness(xDE, yDE, size, K1);
		return fitness;
		};

	double* limits = new double[numVertices * 2];

	for (int i = 0; i < numVertices * 2; i++) {
		if (i % 2) {
			limits[i] = 30;
		}
		else {
			limits[i] = -30;
		}
	}


	DiffEvolution DE(func, limits, numVertices, "targetToBest1", "max");
	DE.startSearch(0.01, 0.5, 0.5, 20, 20);
	int i = 0;
	double* coef = DE.getBestCoordinates();
	changeCoef(coef, i);
	calcFitness(xDE, yDE, size, K1);

	delete[] yDE;

	for (int i = 0; i < size; i++) {
		delete[] xDE[i];
	}
	delete[] xDE;
}
