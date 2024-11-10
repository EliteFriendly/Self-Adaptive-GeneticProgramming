#pragma once
#include <iostream>
#include<vector>
#include<functional>
#include "MutationGP.h"
#include"CrossoverGP.h"
#include"Tree.h"
#include "SelectionGP.h"
#include "FormingGP.h"
#include <thread>
class AdaptiveGeneticProgramming
{
private:
	double K1;//Коэффициент используемый для подсчета пригодности, чем выше тем меньше будут деревья
	int treeDepth;
	Tree bestIndividual;
	int numIndividuals;
	int numGeneration;
	Tree* arrayIndividuals = nullptr;
	Tree* arrayChildren = nullptr;
	double socialCard = 0.1;//Минимальная вероятность выбора

	double** x;//Двойной массив
	int ammInputs;//Количество осей или входов
	double* y;
	int size;//Количество точек


	SelectionGP** selection = nullptr;
	/*
	0-ранговая
	1-пропорциональная
	2-4 турнирная(3,5,10)
	*/
	double* selProbabilities = nullptr;
	int* chosenSel = nullptr;


	CrossoverGP** crossover = nullptr;
	/*
	0-пустое
	1-стандарт
	2-одноточечное
	3-равномерное
	*/
	double* crossProbabilities = nullptr;
	int* chosenCross = nullptr;


	MutationGP** mutation = nullptr;
	/*
	0-дерево
	1-слабая
	2-средняя
	3-сильная
	*/
	double* mutProbabilities = nullptr;
	int* chosenMut = nullptr;


	FormingGP forming;

	void findBest();

	int probabilityChoice(double* arrProb, int size) {
		mt19937 gen(rand());

		double sum = arrProb[0];
		int number = 0;
		double probability = gen() % 1000000 / 1000000.0;
		while (probability > sum) {
			number++;
			if (number == size) {
				return number - 1;
			}
			sum += arrProb[number];
		}
		return number;
	}

	void setSelectionsArrays();

	Tree createChild(int);

	void threadsFitnessCalc(int ammThread);

	int findWinner(int* arr, int ammPlayer,double* arrFitness);

	void changeProbabilities(double* arrProba, int numWinner, int ammPlayers);
	
	void recalcProbabilities();


public:
	AdaptiveGeneticProgramming(double K1, int treeDepth) :K1(K1), treeDepth(treeDepth){
		
		selection = new SelectionGP*[5];
		selection[0] = new RankedSelection;
		selection[1] = new ProportionalSelection;
		selection[2] = new TournamentSelection(3);
		selection[3] = new TournamentSelection(5);
		selection[4] = new TournamentSelection(10);

		crossover = new CrossoverGP * [4];
		crossover[0] = new EmptyCrossover;
		crossover[1] = new StandartCrossover;
		crossover[2] = new OnepointCrossover;
		crossover[3] = new UniformCrossover;

		mutation = new MutationGP * [4];
		mutation[0] = new TreeMutation(treeDepth - 1);
		mutation[1] = new PointMutation("Weak");
		mutation[2] = new PointMutation("Average");
		mutation[3] = new PointMutation("Strong");


		selProbabilities = new double[5];
		crossProbabilities = new double[4];
		mutProbabilities = new double[4];

		for (int i = 0; i < 5; i++) {
			selProbabilities[i] = 1.0 / 5;
		}
		for (int i = 0; i < 4; i++) {
			crossProbabilities[i] = 0.3;
			mutProbabilities[i] = 1.0 / 4;
		}
		crossProbabilities[0] = 0.1;//Ибо там пустой кроссовер
	}
	void startTrain(double** x, int ammInputs, double* y, int size, int numIndividuals, int numGeneration);
	Tree getBest() {
		return bestIndividual;
	}
	double getError(double** x, double *y, int size) {
		double sum = 0;//Среднеквадратичная ошибка
		for (int i = 0; i < size; i++) {
			sum += pow(bestIndividual.getValue(x[i]) - y[i], 2);
		}
		sum = pow(sum / size, 0.5);
		return sum;

	}

	~AdaptiveGeneticProgramming() {
		if (arrayIndividuals != nullptr) {
			delete[] arrayIndividuals;
		}
		if (arrayChildren != nullptr) {
			delete[] arrayChildren;
		}
	}
};

