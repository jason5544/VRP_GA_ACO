#pragma once

#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

class Graph
{
public:
	double **g;
	double *demand;
	int nodeCount;
	Graph(void);
	~Graph(void);

	void readGraph(const char *filePath)
	{
		fstream file_in(filePath);
		if (!file_in)
		{
			cerr << "Open file " << filePath << " error" << endl;
			abort();
		}

		g = new double *[nodeCount];
		for (int i = 0; i < nodeCount; i++)
		{
			g[i] = new double[nodeCount];
		}


		double *x_Ary = new double [nodeCount];
		double *y_Ary = new double [nodeCount];

		//fstream file_in(filePath);

		for (int i = 0; i < nodeCount; i++)
		{
			file_in >> x_Ary[i] >> y_Ary[i];
		}



		//计算两两城市间距离
		double dbTemp=0.0;
		for (int i = 0;i < nodeCount; i++)
		{
			for (int j = 0;j < nodeCount; j++)
			{
				dbTemp = (x_Ary[i]-x_Ary[j])*(x_Ary[i]-x_Ary[j])+(y_Ary[i]-y_Ary[j])*(y_Ary[i]-y_Ary[j]);
				dbTemp = pow(dbTemp, 0.5);
				g[i][j] = (double)((int)(dbTemp + 0.5));
			}
		}
	}

	void readDemand(const char *filePath)
	{
		fstream file_in(filePath);
		if (!file_in)
		{
			cerr << "Open file " << filePath << " error" << endl;
			abort();
		}

		demand = new double[nodeCount];
		fstream file_in_demand(filePath);
		for (int i = 0; i < nodeCount; i++)
		{
			file_in_demand >> demand[i];
		}
	}

};

