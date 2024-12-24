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

	clustering(x, size);

	int* sizeClust = new int[numCluster];
	for (int i = 0; i < numCluster; i++) {
		sizeClust = 0;
	}


	for (int i = 0; i < size; i++) {
		sizeClust[label[i] - 1]++;
	}


	for (int i = 0; i < size; i++) {
		error += pow(y[i] - getValue(x[i]), 2);
	}
	fitness = (1 / (1 + pow(error, 0.5))) * (20 - K1 * numNodes);
	if (fitness == NULL) {
		cout << "������ ����� NAN";
		exit(0);
	}
	delete[] sizeClust;
}

Tree::Tree(int d, int numInputs)
{
	ammInputs = numInputs;
	//������ ���� ����� �� ������ �����
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
		//� ������ ���� �������
		unarFuncUs = true;
		
		numberFunc = rand() % (unarFunc.size());
		//Tree a(d - 1);
		right = new Tree(d-1,numInputs);
	}
	else {
		//� ������ ���� ��������
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
	//���������� ����� ����������� ����� �������
	if (left != nullptr) {//���� ������� �� ����� ������� �� �����
		left->changeCoef(in, z);
	}
	if (right != nullptr) {//���� ��� ������ �����
		right->changeCoef(in, z);
	}
	if (lastVertice and numVertices == 1) {
		coef = in[z];//������ ������������� � ������ ���� ��� ��
		z++;//������ � �������!!!
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
	//if (right != nullptr and left ==nullptr) {//���� ������ ��� �� ���� �� ��� ����� ������� ������

	//	return unarFunc[numberFunc](right->getValue(x));
	//	
	//}
	if (lastVertice) {//���� ����� �� �������
		if (numVertices==1) {
			return coef;
		}
		if (numVertices==0) {
			return x[numInput];
		}
		else {
			cout << "���������������� � getValue";
			exit(0);
		}
	}

	if (unarFuncUs) {
		return unarFunc[numberFunc](right->getValue(x));
	}
	else {//���� �������� � �������� �������
		return binaryFunc[numberFunc](left->getValue(x),right->getValue(x));
	}
	
}

void Tree::replaceNode(int search, Tree& newNode)//������ ���������� ����
{
	if (numNodes == search) {//���� �� ����� �� ���� ��� ����� �� �������
		*this = newNode;
		return;
	}
	//����� �� ������ ����� ���� �� ����� ����������� ������
	if (left != nullptr and search <= left->getNumNodes()) {
		left->replaceNode(search, newNode);
	}
	if (right != nullptr and search <= right->getNumNodes()) {
		right->replaceNode(search, newNode);
	}
}

void Tree::changeNode(int search, Tree& newNode)//������� �� replace � ���, ��� �� �������� ��������� ����
{
	if (numNodes == search) {//���� �� ����� �� ���� ��� ����� �� �������
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
				left->numNodes = -1;//������� ��� ����, ����� �� ���� ��������� � ���� �����
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
	if (label == nullptr) {
		label = new int(size);
	}



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
	DE.startSearch(0.01, 0.5, 0.5, 50, 50);
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




double Tree::distanceAverageIn(double** data, int* sizeClust, int str, int col, int cluster) {

	double sum = 0, dist = 0;

	for (int i = 0; i < str; i++) {
		for (int j = 0; j < str; j++) {
			if (label[i] != label[j] or label[i] != cluster)
				continue;
			dist = 0;
			for (int w = 0; w < col; w++) {

				dist += pow(data[i][w] - data[j][w], 2);

			}
			dist = pow(dist, 0.5);
			sum += dist;



		}
	}

	sum = (sum / (sizeClust[cluster-1])) / (sizeClust[cluster - 1] - 1);

	return sum;

}
double Tree::distanceAverageOut(double** data, int* sizeClust, int str, int col) {

	double sum = 0, dist = 0;


	for (int i = 0; i < str; i++) {
		for (int j = 0; j < str; j++) {
			if (label[i] == label[j])
				continue;

			dist = 0;
			for (int w = 0; w < col; w++) {

				dist += pow(data[i][w] - data[j][w], 2);

			}
			dist = pow(dist, 0.5);
			sum += dist;



		}
	}

	for (int i = 0; i < numCluster; i++) {
		sum /= sizeClust[i];
	}

	return sum;

}

void Tree::clustering(double** data, int str)
{
	
	if (numCluster == 2) {

		for (int i = 0; i < str; i++) {
			if (getValue(data[i]) > 0) {
				label[i] = 1;
			}
			else {
				label[i] = 2;
			}
		}



	}



}
