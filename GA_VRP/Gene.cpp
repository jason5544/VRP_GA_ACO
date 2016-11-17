#include "StdAfx.h"
#include "Gene.h"
#include <algorithm>
#include <ctime>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string.h>
#include "Vehicle.h"


const double PI = 3.1415926;
using namespace std;

int Gene::num;				// 基因数目
int Gene::bitNum;			// 基因bit位数

double Gene::minFitness = 9999;
double Gene::minUnDiveryRate;

vector< vector<int> > Gene::BestVehicleGroup;;

Gene::Gene(void)
{
	geneSequence = new int[Gene::bitNum];
}

Gene::~Gene(void)
{
	delete[] geneSequence;
}

// 初始化
void Gene::init()
{
	for(int i = 0; i < Gene::bitNum; i++)
	{	
		geneSequence[i] = rand()%Gene::bitNum;
		for (int j = 0; j < i; j++)
		{
			if (geneSequence[i] == geneSequence[j])
			{
				i--;
				break;
			}
		}
	}
}

// 交叉
void Gene::swapGene(const Gene &gene0, const Gene &gene1, Gene &gene2, Gene &gene3)
{
	int pos = rand()%Gene::bitNum;
	for (int i = 0; i < pos; i++ )
	{
		gene2.geneSequence[i] = gene0.geneSequence[i];
		gene3.geneSequence[i] = gene1.geneSequence[i];
	}

	for (int i = pos; i < Gene::bitNum; i++ )
	{
		gene2.geneSequence[i] = gene1.geneSequence[i];
		gene3.geneSequence[i] = gene0.geneSequence[i];
	}
}

// 变异
void Gene::variation()
{
	int pos = rand()%Gene::bitNum;
	geneSequence[pos] = rand()%(Gene::bitNum - pos);
}

// 计算fitness
void Gene::fitnessFunction(const Graph &graph)
{
	//cout << "-------------- Gene::bitNum ------------------" << endl;
	//for (int i = 0; i < Gene::bitNum; i++)
	//{
	//	cout << geneSequence[i] << " ";
	//}
	//cout << endl;
	//cout << "-------------- Gene::bitNum ------------------" << endl;
	double unDelivery = 0;
	fitness = 0;
	double sum = 0;
	for (int i = 0 ; i < vehicleGroup.size(); i++)
	{
		delete &vehicleGroup[i];
	}
	vehicleGroup.clear();
	bool tag = 0;
	Vehicle *vehicle = new Vehicle;
	for (int i = 0; i < Gene::bitNum; i++)
	{
		//cout << "i = " << i << endl;
		sum += graph.demand[geneSequence[i] + 1];
		if (sum < Vehicle::capacity)
		{
			if (!tag)
			{
				fitness += graph.g[0][geneSequence[i] + 1];
				tag = 1;
			}
			else
			{
				fitness += graph.g[geneSequence[i - 1] + 1][geneSequence[i] + 1];
			}
			vehicle->supplyNode.push_back(geneSequence[i] + 1);
			//cout << "geneSequence[i] = " << geneSequence[i] << endl;
			//cout << "vehicle.supplyNode.size() = " << vehicle->supplyNode.size() << endl; 
		}
		else
		{
			fitness += graph.g[geneSequence[i - 1] + 1][0];
			unDelivery += graph.g[geneSequence[i - 1] + 1][0];
			vehicleGroup.push_back(*vehicle);
			vehicle = new Vehicle;
			i--;
			sum = 0;
			tag = 0;
		}
	}
	fitness += graph.g[geneSequence[Gene::bitNum - 1] + 1][0];
	unDelivery += graph.g[geneSequence[Gene::bitNum - 1] + 1][0];
	unDeliveryRate = unDelivery/fitness;

	//cout << "需要车的数量为: " << vehicleGroup.size() << endl;
	//for (int i = 0; i < vehicleGroup.size(); i++)
	//{
	//	cout << "车 " << i << " 配货节点 : ";
	//	for (int j = 0; j < vehicleGroup[i].supplyNode.size(); j++)
	//	{
	//		cout << vehicleGroup[i].supplyNode[j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << " 配送车辆所行驶的总路程为: " << fitness << endl;
}


	

// 选择
void Gene::randSwap(Gene *&gene)
{
	double sumFitness = 0;
	//Gene::minFitness = 9999;
	for (int i = 0 ; i < Gene::num; i++)
	{
		if (gene[i].fitness < Gene::minFitness)
		{
			Gene::minUnDiveryRate = gene[i].unDeliveryRate;
			Gene::minFitness = gene[i].fitness;
			for (int i = 0; i < Gene::BestVehicleGroup.size(); i++)
			{
				Gene::BestVehicleGroup[i].clear();
			}
			Gene::BestVehicleGroup.clear();
			Gene::BestVehicleGroup.resize(gene[i].vehicleGroup.size());
			for (int j = 0; j < gene[i].vehicleGroup.size(); j++)
			{
				Gene::BestVehicleGroup[j].resize(gene[i].vehicleGroup[j].supplyNode.size());
				for (int k = 0; k < gene[i].vehicleGroup[j].supplyNode.size(); k++)
				{
					Gene::BestVehicleGroup[j][k] = gene[i].vehicleGroup[j].supplyNode[k];
				}
			}
		}
		sumFitness += gene[i].fitness;
	}
	
	double sumPosibility = 0;
	for (int i = 0; i < Gene::num; i++)
	{
		gene[i].posibility =1 - (double)gene[i].fitness/(double)sumFitness;
		sumPosibility += gene[i].posibility;
	}

	for (int i = 0; i < Gene::num; i++)
	{
		gene[i].posibility = (double)gene[i].posibility/(double)sumPosibility;
	}

	Gene *newGene = new Gene[Gene::num];


	for (int i = 0; i < Gene::num; i = i + 2)
	{
		int order0, order1;
		
		Gene::randSelect(gene , order0, order1);		
		Gene::swapGene(gene[order0], gene[order1], newGene[i], newGene[i + 1]);
	}


	delete[] gene;
	gene = newGene;
}

// 根据fitness随机选择两个基因序列
void Gene::randSelect(Gene *gene, int &order0 ,int &order1)
{
	double randNum0, randNum1;
	get_rand(randNum0, randNum1);

	//cout << "randNum0 = " << randNum0 << " " << "randNum1 = " << randNum1 << endl;
	for (int i = 0; i < Gene::num; i++)
	{
		if (randNum0 < gene[i].posibility)
		{
			order0 = i;
			break;
		}
		else
		{
			randNum0 = randNum0 - gene[i].posibility;
		}
	}

	for (int i = 0; i <Gene::num; i++)
	{
		if (randNum1 < gene[i].posibility)
		{
			order1 = i;
			break;
		}
		else
		{
			randNum1 = randNum1 - gene[i].posibility;
		}
	}

	//cout << "oder0 = " << order0 << " " << "order1 = " << order1 << endl; 
}

// 产生两个随机数
void Gene::get_rand(double &x, double &y)
{
	int N = 999;
	x = rand()%(N+1)/(float)(N+1);         //生成0-1间的随机数
	y = rand()%(N+1)/(float)(N+1);         //生成0-1间的随机数
}